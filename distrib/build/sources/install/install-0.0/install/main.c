/* SmoothWall install program.
 *
 * This program is distributed under the terms of the GNU General Public
 * Licence.  See the file COPYING for details.
 *
 * (c) Lawrence Manning, 2001
 *
 * filename: main.c
 * Contains main entry point, and misc functions. */

#include "install.h"

#define CDROM_INSTALL 0
#define URL_INSTALL 1

FILE *flog = NULL;
char *logname;

char **ctr;

extern char *english_tr[];

int main(int argc, char *argv[])
{
  int sddneeded = 0;
  struct storagedevicedriver sdd;
  struct blockdevice hd, cdrom;	/* Params for CDROM and HD */
  struct blockdevice hdArray[6], cdromArray[6];	/* Arrays of found devices */
  int cdmounted;			/* Loop flag for inserting a cd. */ 
  int rc;
  int skip_dev = 1;
  char commandstring[STRING_SIZE];
  int installtype = CDROM_INSTALL; 
  char *insertmessage, *insertdevnode;
  char tarballfilename[STRING_SIZE],
       devballfilename[STRING_SIZE],
       docballfilename[STRING_SIZE];
  char shortlangname[10];
  int allok = 0;
  struct keyvalue *ethernetkv = initkeyvalues();
  FILE *handle;
  char filename[STRING_SIZE];
  int maximum_free, current_free;
  int log_partition, boot_partition, root_partition, swap_partition;
  struct keyvalue *hwprofilekv = initkeyvalues();
  FILE *hkernelcmdline;
  char kernelcmdline[STRING_SIZE];
  int ramsize;

  newtComponent installConfig, cfgHD, cfgCD, cfgDev, cfgPart;
  char tmpstr[STRING_SIZE*10];

  setenv("TERM", "linux", 0);

  sethostname("smoothwall", 10);

  memset(&sdd, 0, sizeof(struct storagedevicedriver));
  memset(&hd, 0, sizeof(struct blockdevice));
  memset(&cdrom, 0, sizeof(struct blockdevice));

  memset(kernelcmdline, 0, STRING_SIZE);

  /* Log file/terminal stuff. */
  if (argc >= 2)
  {		
    if (!(flog = fopen(argv[1], "w+")))
      return 0;
  }
  else
    return 0;
  
  logname = argv[1];
  
  fprintf(flog, "Install program started.\n");

  if (!(hkernelcmdline = fopen("/proc/cmdline", "r")))
    return 0;
  fgets(kernelcmdline, STRING_SIZE - 1, hkernelcmdline);
  fclose(hkernelcmdline);

  /* Load USB keyboard modules so dialogs can respond to USB-keyboards */
  fprintf(flog, "Loading USB-keyboard modules.\n");
  mysystem("/sbin/modprobe usbcore");
  mysystem("/sbin/modprobe ohci-hcd");
  mysystem("/sbin/modprobe uhci-hcd");
  mysystem("/sbin/modprobe usbhid");

  newtInit();
  newtCls();
  
  ctr = english_tr;
  strcpy(shortlangname, "en");
      
  newtDrawRootText(0, 0, " SmoothWall Express 3.0: Phaeton Development Project -- http://smoothwall.org/");
  newtPushHelpLine(ctr[TR_HELPLINE]);

  newtWinMessage(TITLE, ctr[TR_OK], ctr[TR_WELCOME]);

  installConfig = newtForm(NULL, NULL, 0);
  cfgHD = newtForm(NULL, NULL, 0);
  cfgCD = newtForm(NULL, NULL, 0);
  cfgDev = newtForm(NULL, NULL, 0);
  cfgPart = newtForm(NULL, NULL, 0);

  /* Loop, forward or backward as desired, through forms */
  while (true)
  {
    switch (knob)
    {
      case 1:
        /* Hard Drive Selection */

        /* open /tmp/hd.list, fetch drives, make radiobuttons */
        fetchdevices(&hdArray,"/tmp/hd.list");
        newFormAddComponent(cfgHD, newtLabel(2, 1, "Select Hard Drive");
        for (devArrayIdx=0; devArrayIdx<6; devArrayIdx++)
        {
          if (strlen(hdArray[devArrayIdx].devnode) == 0) break;
          strcpy(tmpstr, hdArray[devArrayIdx].devnode);
          strcat(tmpstr, " - ");
          strcat(tmpstr, hdArray[devArrayIdx].fmtpartdevnode);
          newFormAddComponent(cfgHD,
                              newtRadioButton(4,
                                              devArrayIdx+2,
                                              tmpstr,
                                              devArrayIdx==0?1:0,
                                              NULL));
        }
        newFormAddComponent(cfgHD, newtButton(2, devArrayIdx+2, "OK");
        newFormAddComponent(cfgHD, newtButton(18, devArrayIdx+2, "Go Back");
        newFormAddComponent(cfgHD, newtButton(34, devArrayIdx+2, "Cancel");
        switch (rc)
        {
          case 1:
            /* Fetch selected radio button into (int) harddisk. */
            knob++;
            break;
          default:
            errorbox(ctr[TR_INSTALLATION_CANCELED]);
            goto EXIT;
        }
        break;

      case 2:
        /* CD/DVD Drive Selection */
        int cdradfirst=1;

        /* open /tmp/cd.list, fetch drives, make radiobuttons */
        rc = newtWinChoice(TITLE, ctr[TR_OK], ctr[TR_CANCEL], "Go Back", "Select CD/DVD Drive");
        switch (rc)
        {
          case 1:
            /* Fetch selected radio button into (int) cdrom. */
            knob++;
            break;
          case 3:
            knob--;
            break;
          default:
            errorbox(ctr[TR_INSTALLATION_CANCELED]);
            goto EXIT;
        }
        break;

      case 3:
        /* Use Whole or Partial Disk */
        int cdradfirst=1;

        /* make radiobuttons for 'use whole disk' and 'use ONLY 10GB' */
        rc = newtWinChoice(TITLE, ctr[TR_OK], ctr[TR_CANCEL], "Go Back", "Use Whole Disk?");
        switch (rc)
        {
          case 1:
            /* Fetch selected radio button into (int) partitioning */
            knob++;
            break;
          case 3:
            knob--;
            break;
          default:
            errorbox(ctr[TR_INSTALLATION_CANCELED]);
            goto EXIT;
        }
        break;

      case 4:
        /* Install Development Packages? */
        int cdradfirst=1;

        rc = newtWinChoice(TITLE, ctr[TR_YES], ctr[TR_NO], ctr[TR_CANCEL], "Go Back", "Install Development Packages?");
        switch (rc)
        {
          case 1:
            instdev = 1; /* Yes, install dev pkgs */
            knob++;
            break;
          case 2:
            instdev = 0; /* No, do NOT install dev pkgs */
            knob++;
            break;
          case 4:
            knob--;
            break;
          default:
            errorbox(ctr[TR_INSTALLATION_CANCELED]);
            goto EXIT;
        }
        break;

      case 5:
        /* EXT3 or ReiserFS? */
        int cdradfirst=1;

        /* make radiobuttons for 'EXT3' and 'ReiserFS' */
        rc = newtWinChoice(TITLE, ctr[TR_OK], ctr[TR_CANCEL], "Go Back", "Select Filesystem");
        switch (rc)
        {
          case 1:
            /* Fetch selected radio button into (int) filesys */
            knob++;
            break;
          case 3:
            knob--;
            break;
          default:
            errorbox(ctr[TR_INSTALLATION_CANCELED]);
            goto EXIT;
        }
        break;
    }
    if (knob > 5) break;
  }
  exit;

  if (installtype == CDROM_INSTALL)
  {
    insertmessage = ctr[TR_INSERT_CDROM];
    insertdevnode = cdrom.devnode;

    /* Try to mount /cdrom in a loop. */
    cdmounted = 0;
    snprintf(commandstring, STRING_SIZE, "/bin/mount %s /cdrom", insertdevnode);
    while (!cdmounted)
    {
      rc = newtWinChoice(TITLE, ctr[TR_OK], ctr[TR_CANCEL], insertmessage);
      if (rc != 1)
      {
        errorbox(ctr[TR_INSTALLATION_CANCELED]);
        goto EXIT;
      }
      if (!(mysystem(commandstring)))
        cdmounted = 1;
    }
  }

  rc = newtWinChoice(TITLE, ctr[TR_OK], ctr[TR_CANCEL],
    ctr[TR_PREPARE_HARDDISK], hd.devnode);
  if (rc != 1)
    goto EXIT;

  rc = newtWinChoice(ctr[TR_PREPARE_HARDDISK_WARNING], ctr[TR_OK], ctr[TR_CANCEL],
    ctr[TR_PREPARE_HARDDISK_WARNING_LONG]);
  if (rc != 1)
    goto EXIT;

  rc = newtWinChoice(ctr[TR_SKIP_DEV], ctr[TR_YES], ctr[TR_NO], ctr[TR_SKIP_DEV_TEXT]);
  if (rc != 1) skip_dev = 0;

  /* If this fails, ramsize will be set to 0.  We can still cope though as this
   * figure is only used as a guide to setting the swap size. */
  ramsize = gettotalmemory();
  fprintf(flog, "%d MB RAM detected.\n", ramsize);

  /* Partition, mkswp, mkfs. */
  /* before partitioning, first determine the sizes of each
   * partition.  In order to do that we need to know the size of
   * the disk. */
  maximum_free = getdisksize(hd.devnode) / 1024;
  
  boot_partition = 20; /* in MB */
  current_free = maximum_free - boot_partition;

  swap_partition = ramsize < 64 ? 64 : ramsize; /* in MB */
  current_free = maximum_free - swap_partition;
  
  log_partition = (current_free / 3) > 20 ? current_free / 3 : 20;
  current_free = maximum_free - log_partition;

  root_partition = current_free;
  fprintf(flog, "boot = %d, swap = %d, log = %d, root = %d\n",
    boot_partition, swap_partition, log_partition, root_partition);

  handle = fopen("/tmp/partitiontable", "w");
  fprintf(handle, ",%d,83,\n,%d,82,\n,%d,83,\n,,83,*\n",
    boot_partition, swap_partition, log_partition);
  fclose(handle);		

  snprintf(commandstring, STRING_SIZE, "/bin/sfdisk -uM %s < /tmp/partitiontable", hd.devnode);
  if (runcommandwithstatus(commandstring, ctr[TR_PARTITIONING_DISK]))
  {
    errorbox(ctr[TR_UNABLE_TO_PARTITION]);
    goto EXIT;
  }
  snprintf(commandstring, STRING_SIZE, "/bin/mke2fs -j %s1", hd.devnode);	
  if (runcommandwithstatus(commandstring, ctr[TR_MAKING_BOOT_FILESYSTEM]))
  {
    errorbox(ctr[TR_UNABLE_TO_MAKE_BOOT_FILESYSTEM]);
    goto EXIT;
  }
  snprintf(commandstring, STRING_SIZE, "/bin/mkswap %s2", hd.devnode);
  if (runcommandwithstatus(commandstring, ctr[TR_MAKING_SWAPSPACE]))
  {
    errorbox(ctr[TR_UNABLE_TO_MAKE_SWAPSPACE]);
    goto EXIT;
  }
  snprintf(commandstring, STRING_SIZE, "/bin/mke2fs -j %s3", hd.devnode);
  if (runcommandwithstatus(commandstring, ctr[TR_MAKING_LOG_FILESYSTEM]))
  {
    errorbox(ctr[TR_UNABLE_TO_MAKE_LOG_FILESYSTEM]);
    goto EXIT;
  }
  snprintf(commandstring, STRING_SIZE, "/bin/mke2fs -j %s4", hd.devnode);	
  if (runcommandwithstatus(commandstring, ctr[TR_MAKING_ROOT_FILESYSTEM]))
  {
    errorbox(ctr[TR_UNABLE_TO_MAKE_ROOT_FILESYSTEM]);
    goto EXIT;
  }
  /* Mount harddisk. */
  snprintf(commandstring, STRING_SIZE, "/sbin/mount %s4 /harddisk", hd.devnode);
  if (runcommandwithstatus(commandstring, ctr[TR_MOUNTING_ROOT_FILESYSTEM]))
  {
    errorbox(ctr[TR_UNABLE_TO_MOUNT_ROOT_FILESYSTEM]);
    goto EXIT;
  }
  mkdir("/harddisk/boot", S_IRWXU|S_IRWXG|S_IRWXO);
  mkdir("/harddisk/var", S_IRWXU|S_IRWXG|S_IRWXO);
  mkdir("/harddisk/var/log", S_IRWXU|S_IRWXG|S_IRWXO);
  
  snprintf(commandstring, STRING_SIZE, "/sbin/mount %s1 /harddisk/boot", hd.devnode);
  if (runcommandwithstatus(commandstring, ctr[TR_MOUNTING_BOOT_FILESYSTEM]))
  {
    errorbox(ctr[TR_UNABLE_TO_MOUNT_BOOT_FILESYSTEM]);
    goto EXIT;
  }
  snprintf(commandstring, STRING_SIZE, "/bin/swapon %s2", hd.devnode);
  if (runcommandwithstatus(commandstring, ctr[TR_MOUNTING_SWAP_PARTITION]))
  {
    errorbox(ctr[TR_UNABLE_TO_MOUNT_SWAP_PARTITION]);
    goto EXIT;
  }
  snprintf(commandstring, STRING_SIZE, "/sbin/mount %s3 /harddisk/var/log", hd.devnode);
  if (runcommandwithstatus(commandstring, ctr[TR_MOUNTING_LOG_FILESYSTEM]))
  {
    errorbox(ctr[TR_UNABLE_TO_MOUNT_LOG_FILESYSTEM]);
    goto EXIT;
  }

  if (installtype == URL_INSTALL)
  {
    /* Network driver and params. */
    if (!(networkmenu(ethernetkv)))
    {
      errorbox(ctr[TR_NETWORK_SETUP_FAILED]);
      goto EXIT;
    }
  }

  /* Either use tarball from cdrom or download. */
  if (installtype == CDROM_INSTALL)
  {
    strncpy(tarballfilename, "/cdrom/smoothwall.tgz", STRING_SIZE);
    strncpy(devballfilename, "/cdrom/smoothdev.tgz", STRING_SIZE);
    strncpy(docballfilename, "/cdrom/smoothdoc.tgz", STRING_SIZE);
  }
  else
  {
    if (!(downloadtarball()))
    {
      errorbox(ctr[TR_NO_TARBALL_DOWNLOADED]);
      goto EXIT;
    }
    strncpy(tarballfilename, "/harddisk/smoothwall.tgz", STRING_SIZE);
    strncpy(devballfilename, "/harddisk/smoothdev.tgz", STRING_SIZE);
    strncpy(docballfilename, "/harddisk/smoothdoc.tgz", STRING_SIZE);
  }
  
  /* unpack... */
  snprintf(commandstring, STRING_SIZE, 
    "/bin/tar -C /harddisk -zxvf %s",
    tarballfilename);
  if (runcommandwithprogress(45, 4, TITLE, commandstring, 15000,
    ctr[TR_INSTALLING_FILES]))
  {
    errorbox(ctr[TR_UNABLE_TO_INSTALL_FILES]);
    goto EXIT;
  }

  if (!skip_dev)
  {
    /* install dev progs */
    snprintf(commandstring, STRING_SIZE, 
      "/bin/tar -C /harddisk -zxvf %s",
      devballfilename);
    if (runcommandwithprogress(45, 4, TITLE, commandstring, 10000,
      ctr[TR_INSTALLING_FILES]))
    {
      errorbox(ctr[TR_UNABLE_TO_INSTALL_FILES]);
      goto EXIT;
    }
    /* install docs 'n' incs */
    snprintf(commandstring, STRING_SIZE, 
      "/bin/tar -C /harddisk -zxvf %s",
      docballfilename);
    if (runcommandwithprogress(45, 4, TITLE, commandstring, 6000,
      ctr[TR_INSTALLING_FILES]))
    {
      errorbox(ctr[TR_UNABLE_TO_INSTALL_FILES]);
      goto EXIT;
    }
  }
  
  
  /* Remove temp tarball if we downloaded. */
  if (installtype == URL_INSTALL)
  {
    if (unlink("/harddisk/smoothwall.tgz"))
    {
      errorbox(ctr[TR_UNABLE_TO_REMOVE_TEMP_FILES]);
      goto EXIT;
    }
  }

  replacekeyvalue(hwprofilekv, "STORAGE_DEVNODE", hd.devnode);
  replacekeyvalue(hwprofilekv, "CDROM_DEVNODE", cdrom.devnode);
  if (strlen(sdd.modulename))
    snprintf(filename, STRING_SIZE - 1, "%s/%s.ko", sdd.path, sdd.modulename);
  else
    strncpy(filename, "", STRING_SIZE - 1);
  replacekeyvalue(hwprofilekv, "STORAGE_DRIVER", filename);
  replacekeyvalue(hwprofilekv, "STORAGE_DRIVER_OPTIONS", sdd.moduleoptions);
  writekeyvalues(hwprofilekv, "/harddisk/" CONFIG_ROOT "/main/hwprofile");

  if (runcommandwithstatus("/bin/chroot /harddisk /sbin/depmod -a",
    ctr[TR_CALCULATING_MODULE_DEPENDANCIES]))
  {
    errorbox(ctr[TR_UNABLE_TO_CALCULATE_MODULE_DEPENDANCIES]);
    goto EXIT;
  }


  if (strlen(sdd.modulename))
  {
    snprintf(commandstring, STRING_SIZE - 1, "/bin/chroot /harddisk /usr/bin/installer/makeinitrd %s/%s.o \"%s\"", sdd.path, sdd.modulename, sdd.moduleoptions);
    if (runcommandwithstatus(commandstring, ctr[TR_SETTING_UP_BOOT_DRIVERS]))
    {
      errorbox(ctr[TR_UNABLE_TO_SETUP_BOOT_DRIVERS]);
      goto EXIT;
    }               
  }
  else
  {
    if (runcommandwithstatus("/bin/chroot /harddisk /usr/bin/installer/makeinitrd", ctr[TR_SETTING_UP_BOOT_DRIVERS]))
    {
      errorbox(ctr[TR_UNABLE_TO_SETUP_BOOT_DRIVERS]);
      goto EXIT;
    }
  }

  if (!(writeconfigs(&hd, ethernetkv, shortlangname)))
  {
    errorbox(ctr[TR_ERROR_WRITING_CONFIG]);
    goto EXIT;
  }

  if (runcommandwithstatus("/harddisk/sbin/lilo -r /harddisk",
    ctr[TR_INSTALLING_LILO]))
  {
    errorbox(ctr[TR_UNABLE_TO_INSTALL_LILO]);
    goto EXIT;
  }
  
  if (installtype == CDROM_INSTALL)
  {
    if (mysystem("/sbin/umount /cdrom"))
    {
      errorbox(ctr[TR_UNABLE_TO_UNMOUNT_CDROM]);
      goto EXIT;
    }

    if (!(ejectcdrom(cdrom.devnode)))
    {
      errorbox(ctr[TR_UNABLE_TO_EJECT_CDROM]);
      goto EXIT;
    }
  }

  if (touchfile("/harddisk/var/smoothwall/patches/available"))
  {
    errorbox("Unable to touch patch list file.");
    goto EXIT;
  }		

  newtWinMessage(ctr[TR_CONGRATULATIONS], ctr[TR_OK],
    ctr[TR_CONGRATULATIONS_LONG]);
    
  allok = 1;
        
EXIT:
  fprintf(flog, "Install program ended.\n");	
  fflush(flog);
  fclose(flog);

  if (!(allok))
    newtWinMessage(TITLE, ctr[TR_OK], ctr[TR_PRESS_OK_TO_REBOOT]);	
  
  newtFinished();
  
  freekeyvalues(ethernetkv);

  if (allok)
  {
    /* /proc is needed by the module checker.  We have to mount it
     * so it can be seen by setup, which is run chrooted. */
    if (system("/sbin/mount proc -t proc /harddisk/proc"))
      printf("Unable to mount proc in /harddisk.");
    else
    {
      if (system("/bin/chroot /harddisk /usr/sbin/setup /dev/tty2 INSTALL"))
        printf("Unable to run setup.\n");
      if (system("/sbin/umount /harddisk/proc"))
        printf("Unable to umount /harddisk/proc.\n");
    }
  }
  
  reboot();

  return 0;
}

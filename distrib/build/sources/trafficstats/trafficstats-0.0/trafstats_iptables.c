// This HAS to be a C prog as iptables source wont complie with the
// C++ compiler

#include <iptables.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <asm/types.h>
#include <sys/stat.h>

#include <linux/pkt_sched.h>
#include "trafstats_iptables.h"


// returns the number of rules up to max
// only works with mangle table.

int fetch_counts(const char *chain, struct iptable_data *dp, int max) {
    unsigned int num = 0;
    const char *thischain;
    const struct ipt_entry *ipt;
    iptc_handle_t handle;

    handle = iptc_init("mangle");
    
    if(handle) {
         
        for (thischain = iptc_first_chain(&handle);
             thischain;
             thischain = iptc_next_chain(&handle)) {
            if(!strcmp(thischain, chain)) {
                // go through rules in this chain  
                
                for (ipt = iptc_first_rule(thischain, &handle),num=0;
                     ipt;
                     ipt = iptc_next_rule(ipt, &handle),num++) {
                    if(!ipt || num >= max)
                        break;
                    
                    
                    dp->pcnt = ipt->counters.pcnt; 
                    dp->bcnt = ipt->counters.bcnt;
                    strncpy(dp->iniface, ipt->ip.iniface, IFNAMSIZ);
                    strncpy(dp->outiface, ipt->ip.outiface, IFNAMSIZ);
                    
                    dp->src.s_addr = ipt->ip.src.s_addr;
                    dp->dst.s_addr = ipt->ip.dst.s_addr;
                    dp->smsk.s_addr = ipt->ip.smsk.s_addr;
                    dp->dmsk.s_addr = ipt->ip.dmsk.s_addr;
                    dp->proto = ipt->ip.proto;
                    dp++; // on to next
                }
                break; // no need to look at other chains
            }
        } 
    
        iptc_free(&handle);
    }
    // number of rules matched if == max then there may be more
    
    return (num);
}

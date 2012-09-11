package Stans::modlib;

our @EXPORT = qw();

our @EXPORT_OK = qw(
		   ifcolormap
		   portsmap
		   protocolmap
		   editcrontab
		   dispaliastab
		 );

our %EXPORT_TAGS = ( standard => [@EXPORT_OK], );

our $VERSION = '1.0';

sub ifcolormap
{
	my (%ifcolors, $line, $ifcolor, $ethdev, $ethcolor);
	
	open(FILE, "/var/smoothwall/portfw/aliases") or die 'Unable to open';
	while ( $line = <FILE> ){
		chomp $line;
		next if ( $line eq "" );
	
		( $ifcolor, $ethdev ) = split /,/, $line;
		if ($ifcolor =~ /^RED/) {
			$ethcolor = "<font color=red>$ifcolor</font>";
		} elsif ($ifcolor =~ /^GREEN/) {
			$ethcolor = "<font color=green>$ifcolor</font>";
		} elsif ($ifcolor =~ /^ORANGE/) {
			$ethcolor = "<font color=orange>$ifcolor</font>";
		} elsif ($ifcolor =~ /^PURPLE/) {
			$ethcolor = "<font color=purple>$ifcolor</font>";
		} else {
			$ethcolor = $ifcolor;
		}
		$ifcolors{$ethdev} = "$ethcolor ($ethdev)";
	}
	close FILE;
	return \%ifcolors;
}

sub portsmap
{
	my %ports;
	my $portfiles = "/var/smoothwall/knownports/*";
	
	foreach my $filenames ( glob $portfiles ){
		unless (open(FILE, $filenames))
		{
			next;
		}

		while ( my $line = <FILE> ){
			chomp $line;
			next if ( $line eq "" );
	
			my ( $name, $value ) = split /,/, $line;

			if ($value eq "0") {
				$ports{$value} = '*';
			} else {
				$ports{$value} = "$name ($value)";
			}
		}
	}
	return \%ports;
}

sub protocolmap
{
	my %protocollist;
	
	open(FILE, "/var/smoothwall/portfw/protolist") or die 'Unable to open protocol list file';
	while ( my $line = <FILE> ){
		chomp $line;
		next if ( $line eq "" );

		my ($protocoln, $proto) = split /,/, $line;	
		$protocollist{$protocoln} = $proto;
	}
	close FILE;
	return \%protocollist;
}





1;
__END__
# Below is stub documentation for your module. You'd better edit it!

=head1 NAME

Stans::modlib - Perl extension for blah blah blah

=head1 SYNOPSIS

  use Stans::modlib;

=head1 DESCRIPTION

Stub documentation for Stans::modlib, created by h2xs. It looks like the
author of the extension was negligent enough to leave the stub
unedited.

=head2 EXPORT

None by default.



=head1 SEE ALSO

Mention other useful documentation such as the documentation of
related modules or operating system documentation (such as man pages
in UNIX), or any relevant external documentation such as RFCs or
standards.

If you have a mailing list set up for your module, mention it here.

If you have a web site set up for your module, mention it here.

=head1 AUTHOR

Stanford Prescott, E<lt>sprescott58@comcast.netE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2011 by Stanford Prescott

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.8.8 or,
at your option, any later version of Perl 5 you may have available.


=cut

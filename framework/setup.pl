#!/usr/bin/perl

# setup.pl
#  a script to set up a new program
#
#  Copyright (C) 2010 janus_wel<janus.wel.3@gmail.com>
#  see LICENSE for redistributing, modifying, and so on.

use strict;
use warnings;
use utf8;

use File::Copy;

my $skeleton_dir = './skeleton';
my $replace_target_l = 'sample';
my $replace_target_u = 'SAMPLE';
my $replace_ext = '.hpp';
my $rename_file = $replace_target_l . $replace_ext;
my $header_dir = '../header';
my $helper_dir = 'helper';
my @headers = qw(getopt.hpp event.hpp typeconv.hpp cor.hpp string.hpp);

# main
main();

sub main {
    die "Specify program name." if scalar @ARGV == 0;

    my $name = $ARGV[0];
    print "Generating codes for: " . $name . "\n";

    mkdir $name, 0755;
    setup($skeleton_dir, $name, $name);

    my $helper = "$name/$helper_dir";
    mkdir $helper;
    foreach my $header (@headers) {
        my $src = "$header_dir/$header";
        my $dst = "$helper/$header";
        print "Copying $src to $dst\n";
        copy($src, $dst);
    }
}

sub setup {
    my ($from_dir, $to_dir, $name) = @_;
    my $name_u = uc $name;
    opendir my($dh), $from_dir or die "Could not open dir '$from_dir': $!";
    for my $entry (readdir $dh) {
        next if $entry =~ /^\.{1,2}$/;

        my $dst_file = ($entry eq $rename_file ? $name . $replace_ext : $entry);
        my $src = "$from_dir/$entry";
        my $dst = "$to_dir/$dst_file";
        if (-d $src) {
            mkdir $dst or die "mkdir '$dst' failed: $!" if not -e $dst;
            setup($src, $dst, $name);
        } else {
            print "Copying $src to $dst\n";
            copy($src, $dst) or die "copy failed: $!";
            system qq(perl -pi.bak -e "s/$replace_target_l/$name/g; s/$replace_target_u/$name_u/g" $dst);
            unlink $dst . ".bak";
        }
    }
    closedir $dh;
    return;
}

BEGIN {
}

END {
}

__END__

=head1 NAME

Example - my template

=head1 SYNOPSIS

example.pl < data.csv

=head1 DESCRIPTION

=head2 Section 1

I<italic>, B<bold>, S<non-broken text>, C<codes>,
L<links|name/"sec">, L<http://www.perl.org/>,
E<lt>, E<gt>, F<file>,

rarely used formats
X<index entry>, Z<>

=head2 Section 2

items

=over 4

=item *

=back

=head1 EXAMPLES

=head1 KNOWN ISSUES

=head1 AUTHOR

janus_wel <janus.wel.3@gmail.com> - L<http://d.hatena.ne.jp/janus_wel/>

=head1 LICENSE

The Artistic License

    L<http://www.perl.com/pub/a/language/misc/Artistic.html>
    L<http://dev.perl.org/perl6/rfc/346.html>

=head1 SEE ALSO

http://d.hatena.ne.jp/janus_wel/

=cut

# vim: ts=4 sw=4 sts=0 et

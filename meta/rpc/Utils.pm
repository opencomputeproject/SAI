# Copyright 2021-present Intel Corporation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

package Utils;

use 5.020;

use File::Basename qw(basename dirname);
use English qw(-no_match_vars);
use Term::ANSIColor;
use Env qw(PATH);
use Carp;

use namespace::autoclean;
use Moose;

# Create standalone application
sub dist {
    my $class  = shift;
    my $script = shift;

    my $pp = 'pp';
    my $app_name = basename( $script, '.pl' );

    croak colored(
        'Cannot replace myself, use the original '
          . basename($script)
          . ' instead',
        'dark yellow'
      )
      if $app_name eq basename($PROGRAM_NAME)
      or $app_name . '-perl' eq basename($PROGRAM_NAME);

    if (
        system("$pp -c -o $app_name $script -a templates -M diagnostics") == 0 )
    {
        say colored( "$app_name",   'italic bold dark green' )
          . colored( ' generated!', 'dark green' )
          . ' You can now move this standalone application into another system';
    }
    else {
        say colored( "$app_name",       'italic bold red' )
          . colored( ' not generated!', 'red' );
    }

    if (
        system(
            "$pp -c -B -P -o $app_name-perl $script -a templates -M diagnostics"
        ) == 0
      )
    {
        say colored( "$app_name-perl", 'italic bold dark green' )
          . colored( ' generated!',    'dark green' )
          . ' You can now move this standalone perl script into another system';
    }
    else {
        say colored( "$app_name",       'italic bold red' )
          . colored( ' not generated!', 'red' );
    }

    return;
}

# Generate readme basing on POD in the source file
# second argument is optional - if provided, additional
# data will be generated in the given directory
sub generate_readme {
    my $class   = shift;
    my $script  = shift;
    my $gen_dir = shift;

    my $pod2text   = 'pod2text';
    my $app_name   = basename( $script, '.pl' );
    my $script_dir = dirname($script);

    system("$pod2text $script > $script_dir/$app_name.README") == 0
      or say "$app_name.README not generated";

    if ($gen_dir) {
        my $pod2html = 'pod2html';

        system("$pod2html $script > $gen_dir/$app_name.html") == 0
          or say "$app_name.html not generated";
    }
    return;
}

__PACKAGE__->meta->make_immutable;
1;

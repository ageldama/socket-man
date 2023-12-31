#!/usr/bin/env perl

use strict;
use warnings;
use feature qw(say);
use utf8;
use open ':encoding(UTF-8)';
use open ':std' => ':encoding(UTF-8)';

# use open IO     => ':encoding(UTF-8)';

use Encode 'encode';

use Socket        qw(IPPROTO_TCP);
use Socket::Linux qw(TCP_KEEPINTVL TCP_KEEPIDLE TCP_KEEPCNT);

use Future::AsyncAwait;
use IO::Async::Loop;
use IO::Async::Listener;
use IO::Socket::INET;
use JSON::XS;
use File::Slurp;
use DDP;
use IO::Async::Timer::Periodic;

# binmode( STDOUT, ":encoding(UTF-8)" );

my $pokemons = decode_json( read_file('pokemon-ko.json') );

sub pokemon {
    return $pokemons->[ rand @$pokemons ];
}

#say pokemon;

my $loop = IO::Async::Loop->new;

my $listener = IO::Async::Listener->new(
    on_stream => sub {
        my ( undef, $stream ) = @_;

        # binmode( $stream->write_handle, ':encoding(UTF-8)' );
        # p($stream);
        # binmode( $stream->write_handle, ":utf8" );

        my $timer = undef;

        $stream->configure(
            on_read => sub {
                my ( $self, $buffref, $eof ) = @_;

                # $self->write($$buffref);
                # $$buffref = "";
                return 0;
            },

            on_closed => sub {
                my ($self) = @_;
                say "Closed.";

                if ($timer) {
                    $loop->remove($timer);
                    $timer->stop;
                }
            },

            # --- no effects / unappliables:

            # on_flushed => sub { say 'Flushed.'; },

            # on_send_error => sub {
            #     my ( $stream, $errno ) = @_;
            #     say 'ERR:', $errno;
            # },
        );

        $loop->add($stream);

        $timer = IO::Async::Timer::Periodic->new(
            interval => 5,

            on_tick => sub {
                my $p = pokemon;
                say $p;
                my $bytes = encode( 'UTF-8', $p );
                my $n     = $stream->write( $bytes . "\n" )->get;
                p($n);
            },
        );

        $timer->start;

        $loop->add($timer);
    },

);

$loop->add($listener);

my $socket = IO::Socket::INET->new(
    LocalHost => '0.0.0.0',
    LocalPort => 1234,
    Listen    => 1,
    ReuseAddr => 1,
    Blocking  => 0
);

setsockopt( $socket, SOL_SOCKET, SO_KEEPALIVE, 1 );

$listener->listen(
    handle    => $socket,
    on_listen => sub {
        my ($listener) = @_;
        my $socket = $listener->read_handle;

        # p $socket;
        # setsockopt( $socket, SOL_SOCKET, SO_KEEPALIVE, 1 );

        setsockopt( $socket, IPPROTO_TCP, TCP_KEEPIDLE,  1 );
        setsockopt( $socket, IPPROTO_TCP, TCP_KEEPINTVL, 2 );
        setsockopt( $socket, IPPROTO_TCP, TCP_KEEPCNT,   2 );

        say "Now listening on port ", $socket->sockhost, ':', $socket->sockport;
    },
);

$loop->run;

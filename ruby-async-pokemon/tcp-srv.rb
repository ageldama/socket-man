#!/usr/bin/env ruby
# frozen_string_literal: true

require 'pokedex'

require 'async'
require 'async/io/trap'
require 'async/io/host_endpoint'
require 'async/io/stream'

endpoint = Async::IO::Endpoint.tcp('0.0.0.0', 1234)

interrupt = Async::IO::Trap.new(:INT)

Async do |top|
  interrupt.install!
  
  endpoint.bind do |server, task|
    Console.logger.info(server) {"Accepting connections on #{server.local_address.inspect}"}
    
    task.async do |subtask|
      interrupt.wait
      
      Console.logger.info(server) {"Closing server socket..."}
      server.close
      
      interrupt.default!
      
      Console.logger.info(server) {"Waiting for connections to close..."}
      subtask.sleep(4)
      
      Console.logger.info(server) do |buffer|
	buffer.puts "Stopping all tasks..."
	task.print_hierarchy(buffer)
	buffer.puts "", "Reactor Hierarchy"
	task.reactor.print_hierarchy(buffer)
      end
      
      task.stop
    end

    server.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, true)
    server.setsockopt(Socket::SOL_SOCKET, Socket::SO_KEEPALIVE, true)
    
    server.listen(128)
    
    server.accept_each do |peer|
      remote_addr = peer.remote_address

      peer.setsockopt(Socket::SOL_TCP, Socket::TCP_KEEPIDLE, 1)
      peer.setsockopt(Socket::SOL_TCP, Socket::TCP_KEEPINTVL, 1)
      peer.setsockopt(Socket::SOL_TCP, Socket::TCP_KEEPCNT, 2)

      Console.logger.info(server) { "Accepted: #{remote_addr.inspect}" }

      stream = Async::IO::Stream.new(peer)

      begin
        # while chunk = stream.read_partial
        loop do
          # Console.logger.debug(self) {chunk.inspect}
          poke = Pokedex::Filter::new.ichooseyou![0]['name']['english']
	  stream.write("#{poke}\n")
	  stream.flush

          Console.logger.info(remote_addr) { "Sent: #{poke}" }

          # KEEPIDLE ..이후 KEEPINTVL 마다, KEEPCNT 횟수만큼 Keep-Alive 확인 실패 => 종료.
          #
          # 즉, SLEEP > KEEPIDLE + (KEEPINTVL x KEEPCNT) ...만큼이어야 keepalive 유효함:
          sleep 5
	  
	  # Console.logger.info(server) do |buffer|
	  # 	task.reactor.print_hierarchy(buffer)
	  # end
        end
      ensure
        Console.logger.info(server) { "Closed: #{remote_addr.inspect}" }
      end
      
    end
  end
end

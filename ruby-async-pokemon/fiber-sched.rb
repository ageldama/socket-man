

require 'async/scheduler'

scheduler = Async::Scheduler.new
Fiber.set_scheduler(scheduler)

Fiber.schedule do
  1.upto(3) do |i|
    Fiber.schedule do
      sleep 1
      puts "Hello World"
    end
  end
end

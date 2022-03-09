module Nuklear
  class Context
    include Nuklear::UI::Container
    attr_accessor :renderer
    attr_reader :events

    def events
      @events ||= EventBuffer.new
    end

    def process_commands
      remaining = commands.dup
      while remaining.any?
        cmd = remaining.shift
        # cmd.tick(delta) if cmd.respond_to?(:tick)
        remaining.concat cmd.commands if cmd.respond_to?(:commands)
      end
    end

    # Triggers an event. The event name must be one of the symbols defined
    # in Nuklear::EventBuffer::EVENT_NAMES.
    def trigger(event_name, *event_args)
      events.add(event_name, *event_args)
    end

    def dsl(&block)
      Nuklear::DSL.new(self, &block)
    end

    # Draws the UI. This method will be called at the appropriate time in the
    # Nuklear frame update process. It should not be called directly.
    protected def paint
      run_commands(self)
      renderer.render
    end

    # Events (window events, input events, etc) will accumulate over time.
    # Nuklear requires these events to be processed at the proper time or else
    # issues could arise. This method will be called at the appropriate time
    # in order to handle the events that have accumulated since the last
    # frame. It should not be called directly.
    protected def process_pending_events
      events.drain do |evt|
        @event_sink.send(*evt)
      end
    end

  end
end

module Nuklear
  # Stores staged events waiting to be drained.
  class EventBuffer
    # These correlate to methods defined by Nuklear::Context::EventSink.
    EVENT_NAMES = [:motion, :key, :button, :scroll, :char, :glyph, :unicode]

    attr_reader :pending_events

    def initialize
      @pending_events = []
    end

    def drain
      @pending_events.each { |evt| yield(evt) }
      @pending_events.clear
    end

    def add(event_name, *event_args)
      super unless EVENT_NAMES.include?(event_name)
      @pending_events << [event_name, *event_args]
    end
  end
end

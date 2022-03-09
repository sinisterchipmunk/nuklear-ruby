module Nuklear
  module UI
    module Events
      def on(*events, &block)
        events.each do |event|
          event_listeners_for(event) << block
        end
      end

      def event_listeners_for(event)
        @event_listeners ||= {}
        @event_listeners[event] ||= []
      end

      def trigger(event)
        if event != @last_event # prevent triggering every frame
          event_listeners_for(event).each { |listener| listener.call event }
          @last_event = event
        end
      end
    end
  end
end

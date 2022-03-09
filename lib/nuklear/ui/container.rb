module Nuklear
  module UI
    module Container
      def commands
        @commands ||= []
      end

      def run_command(context, command)
        if command.respond_to?(:to_command)
          if !command.respond_to?(:enabled?) || command.enabled?
            cmd = command.to_command
            result = send(cmd[0], context, *cmd[1..-1]) do |ret|
              command.result(ret, context) if command.respond_to?(:result)
            end
            command.result(result, context) if !result.nil? && command.respond_to?(:result)
          end
        elsif command.respond_to?(:[])
          send(command[0], context, *command[1..-1])
        else
          raise "Unexpected command #{command.inspect}"
        end
      end

      def run_commands(context)
        commands.each do |command|
          if command.respond_to?(:to_commands)
            command.to_commands.each { |cmd| run_command(context, cmd) }
          else
            run_command(context, command)
          end
        end
      end

      def <<(other)
        commands << other
      end

      # Iterates over every item in this container, and yields each item
      # (starting with the container itself) to the block.
      def traverse(&block)
        yield self
        commands.each do |item|
          if item.respond_to?(:traverse)
            item.traverse(&block)
          else
            yield item
          end
        end
      end

      # Traverses this container and any of its sub-containers, yielding each
      # item to the block. Returns each item for which the block returned true.
      def find(&block)
        matches = []
        traverse do |item|
          matches << item if yield(item)
        end
        return matches
      end

      LayoutRowDynamic = Struct.new(:height, :ncols) do
        def to_command
          [ :ui_layout_row_dynamic, height, ncols ]
        end
      end

      def layout_row_dynamic(height, ncols)
        LayoutRowDynamic.new(height, ncols).tap { |row| commands << row }
      end

      def method_missing(name, *args)
        if respond_to?(:"ui_#{name}")
          commands << [:"ui_#{name}", *args]
        else
          super
        end
      end
    end
  end
end

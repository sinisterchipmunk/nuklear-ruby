require 'nuklear/ui'

module Nuklear
  class DSL
    def initialize(ctx, &block)
      @ctx = ctx
      instance_eval(&block) if block_given?
    end

    def add_component(klass, *args, **kw, &block)
      # if klass initialize accepts a block, pass the block into that. The
      # caller must call .dsl on the result if they want a nested DSL.
      if block_given? && klass.instance_method(:initialize).parameters.last[0] == :block
        instance = klass.new(*args, **kw, &block)
        @ctx << instance
      else # assume block is for a nested DSL
        instance = klass.new(*args, **kw)
        @ctx << instance
        instance.dsl(&block) if block_given?
      end
      instance
    end

    def method_missing(name, *args, **kw, &block)
      @ctx.send(name, *args, **kw, &block)
    end

    def respond_to?(name)
      super || @ctx.respond_to?(name)
    end

    # Scans all descendants of Nuklear::UI::Base and defines DSL method names
    # based on each component name. This happens once automatically at startup
    # but if you are having trouble with a new custom UI component, then you
    # can call this method to make sure it's been defined.
    def self.prepare!
      Nuklear::UI::Base.descendants.each do |descendant|
        define_method descendant.dsl_method_name do |*args, **kw, &block|
          add_component(descendant, *args, **kw, &block)
        end
      end
    end

    prepare!
  end
end

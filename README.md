# Nuklear

Ruby bindings for [Nuklear](https://github.com/Immediate-Mode-UI/Nuklear).

Here is the description from the Nuklear project:

> This is a minimal-state, immediate-mode graphical user interface toolkit
> written in ANSI C and licensed under public domain. It was designed as a
> simple embeddable user interface for application and does not have any
> dependencies, a default render backend or OS window/input handling but
> instead provides a highly modular, library-based approach, with simple
> input state for input and draw commands describing primitive shapes as
> output. So instead of providing a layered library that tries to abstract
> over a number of platform and render backends, it focuses only on the actual
> UI.

This project implements a set of Ruby classes which wrap the Nuklear API. It
also provides two sample renderers (OpenGL 2 and OpenGL 4).


## Installation

Add this line to your application's Gemfile:

```ruby
gem 'nuklear'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install nuklear


## Renderers

The base class `Nuklear::Renderer` is essentially a headless renderer. It
implements all necessary functionality to serve as a renderer for Nuklear,
except the actual drawing, and is also intended to be subclassed for a custom
renderer which actually draws something. The two bundled OpenGL renderers
(`Nuklear::Renderer::OpenGL2` and `Nuklear::Renderer::OpenGL4`) are examples
of this.


## API

The following examples are to help introduce you to how to use Nuklear, and
to inform you as to how Nuklear fits into the larger design of a graphical
application. They are not complete examples. For complete, functional
examples, see the files in /examples/.


## Initialization

```ruby
# Nuklear will load the font files for us, and will generate a font atlas
# image. Then it will yield the width, height and pixel data of that image
# to our block. Typically we would use this information to generate a
# texture for our preferred rendering engine (OpenGL, Vulkan, etc.). Once
# we have a handle, we pass that handle back to Nuklear as the result of
# the block. Here we just use `0`. Nuklear will hold on to the handle,
# and it will be passed back to us as part of draw calls involving the
#font.
font = Nuklear::Font.new("arial.ttf", 12) { |w, h, data| 0 }

# Once we have our font, we can create a Nuklear context.
ctx = Nuklear::Context.new(font)

# The context requires a renderer. We can pass it an instance of
# Nuklear::Renderer, but this will not actually draw anything.
# Nuklear::Renderer is great for unit testing and as a base class for us
# to actually implement our real renderer on top of. Here, we'll use the
# bundled OpenGL 2 renderer.
require 'nuklear/nuklear_renderer_opengl2'
ctx.renderer = Nuklear::Renderer::OpenGL2.new(ctx)
```

### With DSL

For convenience, a DSL is provided. It is particularly useful for building
complex, mostly static interfaces as it removes a lot of the repetition that
would come from using the primary interface. Here is a trivial example:

```ruby
ctx.dsl do
  w = window "Hello Nuklear", left: 10, top: 10, width: 100, height: 100 do
    layout_row_dynamic 35, 2
    button "Click me" do
      puts "Ouch!"
      w.close # hide the window
    end
  end
end
```

See `/examples/calculator.rb` for a more complete demonstration of the DSL.


### Without DSL

You can opt not to use the DSL, and instantiate the UI widgets directly. This
is straightforward, but verbose. It can be helpful when you need to
programmatically build an interface that doesn't need or desire a lot of
syntactic sugar. For example, the DSL might actively hinder you if you were
building an interface by parsing a YAML or XML file. Here is how we would
build the same window as above without use of the DSL:

```ruby
# create the Nuklear window
w = Nuklear::UI::Window.new "Hello Nuklear",
                            left: 10,
                            top: 10,
                            width: 100,
                            height: 100

# Set up the window layout
w.layout_row_dynamic 35, 2

# Add the button to the window
w << Nuklear::UI::Button.new "Click me" do
  puts "Ouch!"
  w.close # hide the window
end

# Add the window to the context
ctx << w
```

See `/examples/hello_nuklear.rb` for a more complete demonstration without the
DSL.


### Main Loop

Since Nuklear is backend-agnostic, you must do the backend stuff yourself. In
all of the examples, you'll find that we use SDL2 for the window management
and event processing, and OpenGL for the actual drawing. You may use whatever
mechanisms you want, including none. (It's quite useful for unit testing to
just use the Nuklear::Renderer base class for "drawing" and to
programmatically deliver only specific window events to Nuklear.)

The event processing loop can be a bit verbose, depending on how many events
you want to respond to, so we'll just show some snippets here. You can find a
more complete example in `/examples/hello_nuklear.rb`.

```ruby
# Initialize the viewport (OpenGL)
glViewport(0, 0, *window.gl_drawable_size)
glClearColor(0, 0, 0, 0)
while true
  # New frame: Clear the screen (OpenGL)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

  # Process window, mouse, keyboard, etc events (SDL2)
  while event = ::SDL2::Event.poll
    case event
    when SDL2::Event::Quit then exit
    # ...
  end

  # Update the Nuklear context. This will generate/update the command buffer
  # in preparation for rendering, then pass that command buffer to our
  # renderer.
  ctx.tick

  # Show the rendered frame. (OpenGL)
  window.gl_swap
end
```


## Development

After checking out the repo, run `bin/setup` to install dependencies. Then,
run `rake test` to run the tests. You can also run `bin/console` for an
interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`.
To release a new version, update the version number in `version.rb`, and then
run `bundle exec rake release`, which will create a git tag for the version,
push git commits and tags, and push the `.gem` file to
[rubygems.org](https://rubygems.org).


## Contributing

Bug reports and pull requests are welcome on GitHub at
https://github.com/sinisterchipmunk/nuklear-ruby.

## License

The gem is available as open source under the terms of the
[MIT License](https://opensource.org/licenses/MIT).

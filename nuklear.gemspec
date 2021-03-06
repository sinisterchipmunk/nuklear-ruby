lib = File.expand_path("lib", __dir__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require "nuklear/version"

Gem::Specification.new do |spec|
  spec.name          = "nuklear"
  spec.version       = Nuklear::VERSION
  spec.authors       = ["Colin MacKenzie IV"]
  spec.email         = ["sinisterchipmunk@gmail.com"]

  spec.summary       = %q{}
  spec.description   = %q{}
  spec.homepage      = "https://github.com/sinisterchipmunk/nuklear-ruby"
  spec.license       = "MIT"

  spec.metadata["allowed_push_host"] = "https://www.rubygems.org"

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = "https://github.com/sinisterchipmunk/nuklear-ruby"
  # spec.metadata["changelog_uri"] = "TODO: Put your gem's CHANGELOG.md URL here."

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files         = Dir.chdir(File.expand_path('..', __FILE__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
  spec.extensions    = ["ext/nuklear/extconf.rb",
                        "ext/nuklear_renderer_opengl2/extconf.rb",
                        "ext/nuklear_renderer_opengl4/extconf.rb"]

  spec.add_development_dependency "bundler", ">= 2.0"
  spec.add_development_dependency "rake", ">= 10.0"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "minitest", ">= 5.0"

  spec.add_development_dependency 'opengl-bindings'
  spec.add_development_dependency 'ruby-sdl2'
end

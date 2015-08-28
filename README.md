Byk
===

[![Gem Version](https://badge.fury.io/rb/byk.svg)](https://rubygems.org/gems/byk)
[![Build Status](https://travis-ci.org/topalovic/byk.svg?branch=master)](https://travis-ci.org/topalovic/byk)

Ruby gem for fast transliteration of Serbian Cyrillic into Latin
<br />
<sub>Inspired by @dejan's
[nice little gem](https://github.com/dejan/srbovanje),
this one comes with a C-optimized twist</sub>

![byk](https://cloud.githubusercontent.com/assets/626128/7155207/07545960-e35d-11e4-804e-5fdee70a3e30.png)


## Installation

If you're using Bundler, add this line to your application's Gemfile:

```ruby
gem "byk"
```

and then execute:

```sh
$ bundle
```

Otherwise, install it yourself:

```sh
$ gem install byk
```


## Usage

Unless automatically required by Bundler, make sure to require the gem
in your initializer:

```ruby
require "byk"
```

This will extend `String` with a couple of simple methods:

```ruby
"Шеширџија".to_latin        # => "Šeširdžija"
"Шеширџија".to_ascii_latin  # => "Sesirdzija"
```

There's also a destructive variant of each:

```ruby
text = "Шеширџија"
text.to_latin!        # => "Šeširdžija"
text                  # => "Šeširdžija"
text.to_ascii_latin!  # => "Sesirdzija"
text                  # => "Sesirdzija"
```

All of these methods take into account the
[digraph capitalization rules](http://sr.wikipedia.org/wiki/Гајица#.D0.94.D0.B8.D0.B3.D1.80.D0.B0.D1.84.D0.B8):

```ruby
"ЉИЉА Љиљановић".to_latin        # => "LJILJA Ljiljanović"
"ĐORĐE Đorđević".to_ascii_latin  # => "DJORDJE Djordjevic"
```


## Safe require

If you prefer not to monkey patch your strings, you can do a "safe"
require in your Gemfile:


```ruby
gem "byk", :require => "byk/safe"
```

or initializer:

```ruby
require "byk/safe"
```

and then rely on module methods:

```ruby
text = "Жвазбука"
Byk.to_latin(text)   # => "Žvazbuka"
text                 # => "Жвазбука"
Byk.to_latin!(text)  # => "Žvazbuka"
text                 # => "Žvazbuka"
```


## Testing

To test the gem, clone the repo and run:

```
$ bundle && bundle exec rake
```


## How fast is fast?

About [10-40x faster](benchmark) than the baseline Ruby implementation
on my hardware, depending on the string's Cyrillic content ratio.

This kind of speed-up might be worthwhile for massive localization
projects, e.g. sites supporting dual script content. Remember,
`Benchmark` is your friend.


## Compatibility

Byk is supported under MRI 1.9.2+. It doesn't build under Rubinius
currently, but I intend to support it in future releases.


## License

This gem is released under the [MIT License](http://www.opensource.org/licenses/MIT).

Уздравље!

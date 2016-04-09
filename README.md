Byk
===

[![Gem Version](https://badge.fury.io/rb/byk.svg)](https://rubygems.org/gems/byk)
[![Build Status](https://travis-ci.org/topalovic/byk.svg?branch=master)](https://travis-ci.org/topalovic/byk)

Ruby gem for fast transliteration of Serbian Cyrillic ↔ Latin

![byk](https://cloud.githubusercontent.com/assets/626128/7155207/07545960-e35d-11e4-804e-5fdee70a3e30.png)


## Installation

Byk can be used as a standalone console utility or as a `String`
extension in your Ruby programs. It has zero dependencies beyond
vanilla Ruby and the toolchain for building native gems <sup>1</sup>.

You can install it directly:

```ruby
$ gem install byk
```

or add it as a dependency in your application's Gemfile:

```ruby
gem "byk"
```

<sub><sup>1</sup> For Windows, you might want to check out
[DevKit](https://github.com/oneclick/rubyinstaller/wiki/Development-Kit)</sub>


## Usage

### As a standalone utility

Here's the help banner with all the available options:

```
usage: byk [options] [files]

options:
  -c, --cyrillic       convert input to Cyrillic (default)
  -l, --latin          convert input to Latin
  -a, --ascii          convert input to "ASCII Latin"
  -v, --version        show version
```

Translation goes to stdout so you can redirect it or pipe it as you
see fit. Let's take a look at some common scenarios.

To translate files to Cyrillic:
```sh
$ byk in1.txt in2.txt > out.txt
```

To translate files to Latin and search for a phrase:
```sh
$ byk -l file.txt | grep stvar
```

Ad hoc conversion:
```sh
$ echo "Вук Стефановић Караџић" | byk -a
Vuk Stefanovic Karadzic
```

or simply omit args and type away:
```sh
$ byk
a u ruke Mandušića Vuka
biće svaka puška ubojita!
^D
а у руке Мандушића Вука
биће свака пушка убојита!
```

`^D` being <kbd>ctrl</kbd> <kbd>d</kbd>.


### As a `String` extension

Unless you're using Bundler, make sure to require the gem in your
initializer:

```ruby
require "byk"
```

This will extend `String` with a couple of simple methods:

```ruby
"Šeširdžija".to_cyrillic    # => "Шеширџија"
"Шеширџија".to_latin        # => "Šeširdžija"
"Шеширџија".to_ascii_latin  # => "Sesirdzija"
```

These do not modify the receiver. For that, there's a destructive
variant of each:

```ruby
text = "Šeširdžija"
text.to_cyrillic!     # => "Шеширџија"
text.to_latin!        # => "Šeširdžija"
text.to_ascii_latin!  # => "Sesirdzija"
text                  # => "Sesirdzija"
```

Note that both latinization methods observe
[digraph capitalization rules](http://sr.wikipedia.org/wiki/Гајица#.D0.94.D0.B8.D0.B3.D1.80.D0.B0.D1.84.D0.B8):

```ruby
"ЉИЉА Љиљановић".to_latin        # => "LJILJA Ljiljanović"
"ĐORĐE Đorđević".to_ascii_latin  # => "DJORDJE Djordjevic"
```


### Safe require

If you prefer not to monkey patch `String`, you can do a "safe"
require in your Gemfile:


```ruby
gem "byk", :require => "byk/safe"
```

or initializer:

```ruby
require "byk/safe"
```

Then, you should rely on module methods:

```ruby
text = "Жвазбука"

Byk.to_latin(text)   # => "Žvazbuka"
text                 # => "Жвазбука"

Byk.to_latin!(text)  # => "Žvazbuka"
text                 # => "Žvazbuka"

# etc.
```


## How fast is "fast" transliteration?

Here's a quick test:

```sh
$ wget https://sr.wikipedia.org/ -O sample
$ du -h sample
128K

$ time byk -l sample > /dev/null
0.08s user 0.04s system 96% cpu 0.126 total
```

Let's up the ante:

```sh
$ for i in {1..800}; do cat sample; done > big
$ du -h big
97M

$ time byk -l big > /dev/null
1.71s user 0.13s system 99% cpu 1.846 total
```

So, ~100MB in under 2s. Fast enough, I suppose. You can expect it to
scale linearly.

Compared to the pure Ruby implementation, it is about
[10-30x faster](benchmark), depending on the input composition and the
transliteration method applied.


## Testing

To test the gem, clone the repo and run:

```
$ bundle && bundle exec rake
```


## Compatibility

Byk is supported under MRI 1.9.2+. I might try my hand in writing a
JRuby extension in a future release.



## License

This gem is released under the [MIT License](LICENSE).

Уздравље!

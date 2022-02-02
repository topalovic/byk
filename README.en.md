Byk
===

[![Gem Version](https://badge.fury.io/rb/byk.svg)](https://rubygems.org/gems/byk)
[![tests](https://github.com/topalovic/byk/actions/workflows/tests.yml/badge.svg)](https://github.com/topalovic/byk/actions/workflows/tests.yml)

Ruby gem for fast transliteration of Serbian Cyrillic to Latin and vice versa

![byk](https://cloud.githubusercontent.com/assets/626128/7155207/07545960-e35d-11e4-804e-5fdee70a3e30.png)

[[srpski, bre!](README.md)]

## Installation

Byk can be used as a standalone console utility or as a `String` extension in your Ruby programs. It has zero dependencies beyond Ruby and the toolchain for building native gems <sup>1</sup>.

You can install it directly:

```ruby
$ gem install byk
```

or add it as a dependency in your application's Gemfile:

```ruby
gem "byk"
```

<sub><sup>1</sup> For Windows, check out [DevKit](https://github.com/oneclick/rubyinstaller/wiki/Development-Kit)</sub>


## Usage

### As a standalone utility

Here's a list of the available options:

```
$ byk --help
usage: byk [options] [files]

options:
  -c, --cyrillic       convert input to Cyrillic (default)
  -l, --latin          convert input to Latin
  -a, --ascii          convert input to "ASCII Latin"
  -v, --version        show version
```

For on-the-fly translation, just feed the string to the command:

```sh
$ byk -a <<< "Вук Стефановић Караџић"
Vuk Stefanovic Karadzic
```

As an alternative, you can omit args and type away, hitting <kbd>Ctrl</kbd> <kbd>D</kbd> once you're done:

```sh
$ byk
a u ruke Mandušića Vuka
biće svaka puška ubojita!
^D
а у руке Мандушића Вука
биће свака пушка убојита!
```

Translation goes to stdout so you can redirect it as you see fit. For example, to translate a plain text file to Cyrillic and save a copy:

```sh
$ byk lat.txt > cyr.txt
```


### As a `String` extension

If you're not using Bundler, make sure to require the gem:

```ruby
require "byk"
```

This will extend `String` with a couple of methods:

```ruby
"Šeširdžija".to_cyrillic   #=> "Шеширџија"
"Шеширџија".to_latin       #=> "Šeširdžija"
"Шеширџија".to_ascii_latin #=> "Sesirdzija"
```

and their destructive variants:

```ruby
text = "Šeširdžija"
text.to_ascii_latin! #=> "Sesirdzija"
text                 #=> "Sesirdzija"
```

#### Safe require

If you prefer not to monkey patch `String`, you can do a "safe" require:

```ruby
# in your Gemfile:
gem "byk", require: "byk/safe"

# otherwise:
require "byk/safe"
```

and then rely on module methods:

```ruby
Byk.to_latin("Жвазбука") #=> "Žvazbuka"
```


## Notes

Note that some exotic cases of digraphs that should be preserved in Cyrillic haven't been taken into account: _Танјуг_, _надживети_, etc.

On the other hand, note that the latinization methods observe [digraph capitalization rules](http://sr.wikipedia.org/wiki/Гајица#.D0.94.D0.B8.D0.B3.D1.80.D0.B0.D1.84.D0.B8):

```ruby
"ЉИЉА Љ. Љиљановић".to_latin       #=> "LJILJA Lj. Ljiljanović"
"ĐORĐE Đ. Đorđević".to_ascii_latin #=> "DJORDJE Dj. Djordjevic"
```


## How fast is "fast"?

Byk is [10-30x faster](benchmark) than the [naive Ruby implementation](https://github.com/dejan/srbovanje), depending on the input composition and the transliteration method applied.


## Testing

To test the gem, clone the repo and run:

```
$ bundle && bundle exec rake
```


## Compatibility

Byk supports Ruby MRI only.


## License

This gem is released under the [MIT License](LICENSE).

Cheers!

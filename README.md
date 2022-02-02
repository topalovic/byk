Byk
===

[![Gem Version](https://badge.fury.io/rb/byk.svg)](https://rubygems.org/gems/byk)

Руби пакет за брзо пресловљавање ћирилице у латиницу и обратно

![byk](https://cloud.githubusercontent.com/assets/626128/7155207/07545960-e35d-11e4-804e-5fdee70a3e30.png)

[[English, please!](README.en.md)]

## Инсталација

Byk се може користити као самостални конзолни алат или као `String` додатак у Руби програмима. Не зависи ни од каквих пакета поред програмског језика Руби и стандардних алата за компајлирање <sup>1</sup>.

Можеш га инсталирати директно:

```ruby
$ gem install byk
```

или додати у `Gemfile` своје апликације:

```ruby
gem "byk"
```

<sub><sup>1</sup> За Windows, види [DevKit](https://github.com/oneclick/rubyinstaller/wiki/Development-Kit)</sub>


## Употреба

### Као самостални алат

Ево прегледа могућности конзолног алата:

```
$ byk --help
usage: byk [options] [files]

options:
  -c, --cyrillic       convert input to Cyrillic (default)
  -l, --latin          convert input to Latin
  -a, --ascii          convert input to "ASCII Latin"
  -v, --version        show version
```

За превод "у лету", проследи текст команди:

```sh
$ byk -a <<< "Вук Стефановић Караџић"
Vuk Stefanovic Karadzic
```

Алтернативно, позови команду, унеси текст за превод и обележи крај уноса помоћу <kbd>Ctrl</kbd> <kbd>D</kbd>:

```sh
$ byk
a u ruke Mandušića Vuka
biće svaka puška ubojita!
^D
а у руке Мандушића Вука
биће свака пушка убојита!
```

Превод се шаље на стандардни излаз, те га можеш преусмерити по потреби. На пример, за превод текстуалне датотеке на ћирилицу и чување превода:

```sh
$ byk lat.txt > cir.txt
```


### Као `String` додатак

У случају да не користиш Bundler, увези пакет "ручно":

```ruby
require "byk"
```

што ће проширити `String` новим методама:

```ruby
"Šeširdžija".to_cyrillic   #=> "Шеширџија"
"Шеширџија".to_latin       #=> "Šeširdžija"
"Шеширџија".to_ascii_latin #=> "Sesirdzija"
```

и њиховим деструктивним верзијама:

```ruby
text = "Šeširdžija"
text.to_ascii_latin! #=> "Sesirdzija"
text                 #=> "Sesirdzija"
```

#### "Безбедан" увоз

У случају да не желиш да проширујеш `String`, можеш извршити "безбедан" увоз:

```ruby
# унутар Gemfile-а:
gem "byk", require: "byk/safe"

# иначе:
require "byk/safe"
```

па се ослонити на методе модула:

```ruby
Byk.to_latin("Жвазбука") #=> "Žvazbuka"
```


## Напомене

Ретки случајеви диграфа које треба сачувати код превођења у ћирилицу нису узети у разматрање: _Танјуг_, _надживети_ и слично.

С друге стране, методе за латинизацију исправно преводе [велике диграфе](http://sr.wikipedia.org/wiki/Гајица#.D0.94.D0.B8.D0.B3.D1.80.D0.B0.D1.84.D0.B8):

```ruby
"ЉИЉА Љ. Љиљановић".to_latin       #=> "LJILJA Lj. Ljiljanović"
"ĐORĐE Đ. Đorđević".to_ascii_latin #=> "DJORDJE Dj. Djordjevic"
```


## Колико брзо је "брзо" пресловљавање?

Byk је [10-30x бржи](benchmark) од [наивне Руби имплементације](https://github.com/dejan/srbovanje), у зависности од састава улаза и смера превођења.


## Тестирање

Клонирај пројекат и покрени тестове:

```
$ bundle && bundle exec rake
```


## Компатибилност

Byk се компајлира под стандардним Рубијем (MRI), почев од верзије 1.9.2.


## Лиценца

Овај пакет је објављен под [МИТ лиценцом](LICENSE).

Уздравље!

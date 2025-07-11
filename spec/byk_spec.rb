# frozen_string_literal: true

require "spec_helper"

describe Byk do

  it "has a version number" do
    expect(Byk::VERSION).not_to be nil
  end

  shared_examples :base do |method|

    # See http://sr.wikipedia.org/wiki/Панграм
    let(:pangram) { +"фијуче ветар у шибљу, леди пасаже и куће иза њих и гунђа у оџацима." }
    let(:pangram_latin) { +"fijuče vetar u šiblju, ledi pasaže i kuće iza njih i gunđa u odžacima." }
    let(:pangram_ascii_latin) { +"fijuce vetar u siblju, ledi pasaze i kuce iza njih i gundja u odzacima." }

    let(:pangram_caps) { +"ФИЈУЧЕ ВЕТАР У ШИБЉУ, ЛЕДИ ПАСАЖЕ И КУЋЕ ИЗА ЊИХ И ГУНЂА У ОЏАЦИМА." }
    let(:pangram_latin_caps) { +"FIJUČE VETAR U ŠIBLJU, LEDI PASAŽE I KUĆE IZA NJIH I GUNĐA U ODŽACIMA." }
    let(:pangram_ascii_latin_caps) { +"FIJUCE VETAR U SIBLJU, LEDI PASAZE I KUCE IZA NJIH I GUNDJA U ODZACIMA." }

    let(:full_cyrillic_coderange) { (0x400..0x4ff).map { |i| i.chr(Encoding::UTF_8) } }
    let(:non_serbian_cyrillic_coderange) { full_cyrillic_coderange - Byk::AZBUKA - Byk::AZBUKA_CAPS }
    let(:non_serbian_cyrillic) { non_serbian_cyrillic_coderange.join }

    let(:ascii) { +"The quick brown fox jumps over the lazy dog." }
    let(:other) { +"संस्कृतम्" }

    let(:mixed) { +"संस्कृतम् илити Sanskrit, obrati ПАЖЊУ." }
    let(:mixed_cyrillic) { +"संस्कृतम् илити Санскрит, обрати ПАЖЊУ." }
    let(:mixed_latin) { +"संस्कृतम् iliti Sanskrit, obrati PAŽNJU." }
    let(:mixed_ascii_latin) { +"संस्कृतम् iliti Sanskrit, obrati PAZNJU." }

    it "doesn't translate an empty string" do
      expect(Byk.send(method, "")).to eq ""
    end

    it "doesn't translate foreign coderanges" do
      expect(Byk.send(method, other)).to eq other
    end
  end

  shared_examples :cyrillization_method do |method|
    include_examples :base, method

    let(:edge_cases) do
      [
        [+"lJ", "љ"],
        [+"nJ", "њ"],
        [+"dŽ", "џ"]
      ]
    end

    it "doesn't translate Cyrillic" do
      expect(Byk.send(method, pangram)).to eq pangram
    end

    it "doesn't translate non-Serbian Cyrillic" do
      expect(Byk.send(method, non_serbian_cyrillic)).to eq non_serbian_cyrillic
    end

    it "translates Latin to Cyrillic" do
      expect(Byk.send(method, pangram_latin)).to eq pangram
    end

    it "translates Latin caps to Cyrillic caps" do
      expect(Byk.send(method, pangram_latin_caps)).to eq pangram_caps
    end

    it "translates mixed text properly" do
      expect(Byk.send(method, mixed)).to eq mixed_cyrillic
    end

    it "translates edge cases properly" do
      edge_cases.each do |input, output|
        expect(Byk.send(method, input)).to eq output
      end
    end

    it "translates ABECEDA to AZBUKA" do
      expect(Byk::ABECEDA.map { |l| l.dup.send(:to_cyrillic) }).to match_array(Byk::AZBUKA)
    end

    it "translates ABECEDA_CAPS to AZBUKA_CAPS" do
      expect(Byk::ABECEDA_CAPS.map { |l| l.dup.send(:to_cyrillic) }).to match_array(Byk::AZBUKA_CAPS)
    end
  end

  shared_examples :latinization_method do |method|
    include_examples :base, method

    let(:edge_cases) do
      [
        [+"ЉЉ", "LJLJ"],
        [+"ЊЊ", "NJNJ"],
        [+"ЏЏ", "DŽDŽ"]
      ]
    end

    it "doesn't translate ASCII" do
      expect(Byk.send(method, ascii)).to eq ascii
    end

    it "doesn't translate Latin" do
      expect(Byk.send(method, pangram_latin)).to eq pangram_latin
    end

    it "doesn't translate non-Serbian Cyrillic" do
      expect(Byk.send(method, non_serbian_cyrillic)).to eq non_serbian_cyrillic
    end

    it "translates Cyrillic to Latin" do
      expect(Byk.send(method, pangram)).to eq pangram_latin
    end

    it "translates Cyrillic caps to Latin caps" do
      expect(Byk.send(method, pangram_caps)).to eq pangram_latin_caps
    end

    it "translates mixed text properly" do
      expect(Byk.send(method, mixed)).to eq mixed_latin
    end

    it "translates edge cases properly" do
      edge_cases.each do |input, output|
        expect(Byk.send(method, input)).to eq output
      end
    end

    it "translates AZBUKA to ABECEDA" do
      expect(Byk::AZBUKA.map { |l| l.dup.send(method) }).to match_array(Byk::ABECEDA)
    end

    it "translates AZBUKA_CAPS to ABECEDA_CAPS" do
      expect(Byk::AZBUKA_CAPS.map { |l| l.dup.send(method) }).to match_array(Byk::ABECEDA_CAPS)
    end
  end

  shared_examples :ascii_latinization_method do |method|
    include_examples :base, method

    let(:edge_cases) do
      [
        [+"Њ", "Nj"],
        [+"Љ", "Lj"],
        [+"Џ", "Dz"],
        [+"Ђ", "Dj"],
        [+"Đ", "Dj"],
        [+"ЊЊ", "NJNJ"],
        [+"ЉЉ", "LJLJ"],
        [+"ЏЏ", "DZDZ"],
        [+"ЂЂ", "DJDJ"],
        [+"ĐĐ", "DJDJ"],
        [+"ЂУРАЂ Ђурђевић", "DJURADJ Djurdjevic"],
        [+"ĐURAĐ Đurđević", "DJURADJ Djurdjevic"]
      ]
    end

    it "doesn't translate ASCII" do
      expect(Byk.send(method, ascii)).to eq ascii
    end

    it "translates Latin to ASCII Latin" do
      expect(Byk.send(method, pangram_latin)).to eq pangram_ascii_latin
    end

    it "translates Latin caps to ASCII Latin caps" do
      expect(Byk.send(method, pangram_latin_caps)).to eq pangram_ascii_latin_caps
    end

    it "translates Cyrillic to ASCII Latin" do
      expect(Byk.send(method, pangram)).to eq pangram_ascii_latin
    end

    it "translates Cyrillic caps to ASCII Latin caps" do
      expect(Byk.send(method, pangram_caps)).to eq pangram_ascii_latin_caps
    end

    it "translates mixed text properly" do
      expect(Byk.send(method, mixed)).to eq mixed_ascii_latin
    end

    it "translates edge cases properly" do
      edge_cases.each do |input, output|
        expect(Byk.send(method, input)).to eq output
      end
    end
  end

  shared_examples :non_destructive_method do |method|
    it "doesn't modify the arg" do
      str = +"ЖŽ"
      expect { Byk.send(method, str) }.to_not change { str }
    end
  end

  shared_examples :destructive_method do |method|
    it "modifies the arg" do
      str = +"ЖŽ"
      expect { Byk.send(method, str) }.to change { str }
    end
  end

  describe ".to_cyrillic" do
    it_behaves_like :cyrillization_method, :to_cyrillic
    it_behaves_like :non_destructive_method, :to_cyrillic
  end

  describe ".to_cyrillic!" do
    it_behaves_like :cyrillization_method, :to_cyrillic!
    it_behaves_like :destructive_method, :to_cyrillic!
  end

  describe ".to_latin" do
    it_behaves_like :latinization_method, :to_latin
    it_behaves_like :non_destructive_method, :to_latin
  end

  describe ".to_latin!" do
    it_behaves_like :latinization_method, :to_latin!
    it_behaves_like :destructive_method, :to_latin!
  end

  describe ".to_ascii_latin" do
    it_behaves_like :ascii_latinization_method, :to_ascii_latin
    it_behaves_like :non_destructive_method, :to_ascii_latin
  end

  describe ".to_ascii_latin!" do
    it_behaves_like :ascii_latinization_method, :to_ascii_latin!
    it_behaves_like :destructive_method, :to_ascii_latin!
  end
end

describe String do
  it "responds to Byk methods" do
    Byk.singleton_methods.each do |method|
      expect("").to respond_to(method)
    end
  end
end

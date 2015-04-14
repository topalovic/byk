# coding: utf-8

require "spec_helper"

describe Byk do

  # See http://sr.wikipedia.org/wiki/Панграм
  let(:pangram) { "фијуче ветар у шибљу, леди пасаже и куће иза њих и гунђа у оџацима." }
  let(:pangram_latin) { "fijuče vetar u šiblju, ledi pasaže i kuće iza njih i gunđa u odžacima." }
  let(:pangram_ascii_latin) { "fijuce vetar u siblju, ledi pasaze i kuce iza njih i gundja u odzacima." }

  let(:pangram_caps) { "ФИЈУЧЕ ВЕТАР У ШИБЉУ, ЛЕДИ ПАСАЖЕ И КУЋЕ ИЗА ЊИХ И ГУНЂА У ОЏАЦИМА." }
  let(:pangram_latin_caps) { "FIJUČE VETAR U ŠIBLJU, LEDI PASAŽE I KUĆE IZA NJIH I GUNĐA U ODŽACIMA." }
  let(:pangram_ascii_latin_caps) { "FIJUCE VETAR U SIBLJU, LEDI PASAZE I KUCE IZA NJIH I GUNDJA U ODZACIMA." }

  let(:ascii) { "The quick brown fox jumps over the lazy dog." }
  let(:other) { "संस्कृतम् saṃskṛtam" }

  let(:mixed) { "संस्कृतम् saṃskṛtam илити Sanskrit, obrati ПАЖЊУ." }
  let(:mixed_latin) { "संस्कृतम् saṃskṛtam iliti Sanskrit, obrati PAŽNJU." }
  let(:mixed_ascii_latin) { "संस्कृतम् saṃskṛtam iliti Sanskrit, obrati PAZNJU." }

  it "has a version number" do
    expect(Byk::VERSION).not_to be nil
  end

  describe "#to_latin" do

    it "doesn't modify an empty string" do
      expect("".to_latin).to eq ""
    end

    it "doesn't modify ASCII text" do
      expect(ascii.to_latin).to eq ascii
    end

    it "doesn't modify latin" do
      expect(pangram_latin.to_latin).to eq pangram_latin
    end

    it "doesn't modify other scripts" do
      expect(other.to_latin).to eq other
    end

    it "converts cyrillic to latin" do
      expect(pangram.to_latin).to eq pangram_latin
    end

    it "converts cyrillic caps to latin caps" do
      expect(pangram_caps.to_latin).to eq pangram_latin_caps
    end

    it "converts mixed text properly" do
      expect(mixed.to_latin).to eq mixed_latin
    end

    it "converts AZBUKA to ABECEDA" do
      expect(Byk::AZBUKA.map(&:to_latin)).to match_array(Byk::ABECEDA)
    end

    it "converts AZBUKA_CAPS to ABECEDA_CAPS" do
      expect(Byk::AZBUKA_CAPS.map(&:to_latin)).to match_array(Byk::ABECEDA_CAPS)
    end
  end

  describe "#to_ascii_latin" do

    # Special care for Њ, Љ, Ђ, Đ
    let(:edge_cases) {
      {
        "Њ" => "Nj",
        "Љ" => "Lj",
        "Ђ" => "Dj",
        "Đ" => "Dj",
        "ЊЊ" => "NJNJ",
        "ЉЉ" => "LJLJ",
        "ЂЂ" => "DJDJ",
        "ĐĐ" => "DJDJ",
        "ГУЊ" => "GUNJ",
        "ПАСУЉ" => "PASULJ",
        "ЂУРАЂ" => "DJURADJ",
        "ĐURAĐ" => "DJURADJ",
        "ĐURAĐ Đorđević" => "DJURADJ Djordjevic",
        "ĐURAĐ. Đorđević" => "DJURADJ. Djordjevic"
      }
    }

    it "doesn't modify an empty string" do
      expect("".to_ascii_latin).to eq ""
    end

    it "doesn't modify ASCII text" do
      expect(ascii.to_ascii_latin).to eq ascii
    end

    it "doesn't modify other scripts" do
      expect(other.to_ascii_latin).to eq other
    end

    it "converts cyrillic to ASCII latin" do
      expect(pangram.to_ascii_latin).to eq pangram_ascii_latin
    end

    it "converts cyrillic caps to ASCII latin caps" do
      expect(pangram_caps.to_ascii_latin).to eq pangram_ascii_latin_caps
    end

    it "converts latin to ASCII latin" do
      expect(pangram_latin.to_ascii_latin).to eq pangram_ascii_latin
    end

    it "converts latin caps to ASCII latin caps" do
      expect(pangram_latin_caps.to_ascii_latin).to eq pangram_ascii_latin_caps
    end

    it "converts mixed text properly" do
      expect(mixed.to_ascii_latin).to eq mixed_ascii_latin
    end

    it "converts edge cases properly" do
      edge_cases.each do |input, output|
        expect(input.to_ascii_latin).to eq output
      end
    end
  end
end

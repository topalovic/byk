class String

  Byk.singleton_methods.each do |method|
    define_method(method) do
      Byk.send(method, self)
    end
  end
end


foo = 0.0
while 1:
    x = input()
    if "Volts" in x:
        new_foo = float(x[:x.find('V')])
    else:
        new_foo = foo
    if foo != new_foo:
        print(new_foo)
    foo = new_foo

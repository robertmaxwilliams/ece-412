

while True:
    line = input()
    for c in line:
        print('/', '{:x}'.format(ord(c)), sep='',  end='')
    print()

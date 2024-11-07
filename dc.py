from dataclasses import dataclass

@dataclass
class Person:
    name: str
    age: int

people = [Person("山田太郎", 25), Person("佐藤花子", 22), Person("鈴木一郎", 30)]

for person in people:
    print(person)
    
def myfunc(var):
    print(type(var))
    for person in var:
        print(type(person))
        print(person)

def myfunc2(var : list[Person]):
    print(type(var))
    for person in var:
        print(type(person))
        print(person)
    return 6

#myfunc(people)

myfunc2(people)

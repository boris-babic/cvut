import random;

active = 1
moznosti = ["Kamen", "Papier", "Noznice"]
skore = [0 , 0] #pc vs hrac
def compare(text1, text2):
    print("pc vybral",text2)
    if text1 == text2:
        print("remiza")
        return 1

    elif (text1 == "kamen"):
        if (text2 == "papier"):
            print("prehral si")
            return 0
        else:
            print("vyhral si")
            return 3
    elif (text1 == "papier"):
        if (text2 == "noznice"):
            print("prehral si")
            return 0
        else:
            print("vyhral si")
            return 3
    elif (text1 == "noznice"):
        if (text2 == "kamen"):
            print("prehral si")
            return 0
        else:
            print("vyhral si")
            return 1



while(active):
    text = input("Zadaj tvoj vyber alebo zadaj koniec  ")
    if text == "koniec":
        active = 0
        print(f"skoncilo to {skore[0]} pre pocitac a {skore[1]} pre hraca")
        break
    else:
        vysledok = compare(text.lower(), random.choice(moznosti).lower())
        if vysledok == 0:
            skore[0] += 1
        elif vysledok == 3:
            skore[1] += 1
            
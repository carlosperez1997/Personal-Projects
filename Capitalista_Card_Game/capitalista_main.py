class Card:
    def __init__(self, number, suit):
        if number == 1 or number == 2:
            self.number = number + 12  # from 1 to 2
        else:
            self.number = number  # from 3 to 12

        self.suit = suit  # Oro, Basto, Espada, Copa

        if self.number > 12:
            string = self.suit + ' - ' + str(self.number - 12)
        else:
            string = self.suit + ' - ' + str(self.number)

        self.name = string

    def print(self):
        print(self.name)

        #print(string)

class start_game:

    def __init__(self):
        self.number_players = self.Numbers_players()
        self.deal_cards()


    def Numbers_players(self):
        print('Select the number of players: ')
        bool = False
        while bool == False:

            Ix = input()

            try:
                I = int(Ix)
                if I > 10:
                    print('Select a reasonable number of players: ')
                else:
                    bool == True
                    break
            except:
                print('Introduce a correct number!')
                bool = False

        return I


    def deal_cards (self):
        import random

        number_each_deck = (14-3)
        #cards_each_deck = range(3, 14, 1)
        cards_each_deck = [3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]

        cards = []
        cards.append(cards_each_deck) #Oro
        cards.append(cards_each_deck) #Basto
        cards.append(cards_each_deck) #Espada
        cards.append(cards_each_deck) #Copa

        decks_available = [1, 2, 3, 4]

        num_cards = len(cards)*len(cards_each_deck)
        p = 0;

        finished = False

        while finished == False:

            if p == num_cards:
                finished = True

            else:

                poss_deck = len(decks_available)-1
                num_deck = random.randint(0, poss_deck)
                poss_num = len(cards[num_deck])-1

                deck = decks_available[num_deck]

                if poss_num == 0:
                    for i in range(poss_deck):
                        if decks_available[i] == deck:
                            del decks_available[i]

                else:
                    num_card = random.randint(0, poss_num)-1
                    NUMBER = cards[num_deck][num_card]
                    DECK = deck_name(deck)

                    print('Card: ', DECK, NUMBER)
                    #print()

                    p = p + 1

                    del cards[num_deck][num_card]


def deck_name (index):
    if index == 1:
        name = 'Oro'
    elif index == 2:
        name = 'Basto'
    elif index == 3:
        name = 'Espada'
    else:
        name = 'Copa'
    return name




def print_hand (hand_cards, to_play):
    print('Your hand is:')
    i = 0
    for card in hand_cards:
        print('(', i+1, ')', ':', card.name, ' : ', toplay[i], ' ; ')
        i = i + 1
    print()


def compare(card_table, card_hand):

    bool = False

    if card_table.number == 7:

        if card_hand.number == 7 or card_hand.number == 8:
            bool = True
        else:
            bool = False

    elif card_table.number == 10:

        if card_hand.number <= 10:
            bool = True
        else:
            bool = False

    elif card_hand.number == 2 and card_hand.suit == 'Oro':

        bool = True

    elif card_hand.number >= card_table.number:

        bool = True

    else:
        bool = False

    #print(bool)
    return bool




if __name__ == '__main__':

    start_game()

    card_table = Card(7, 'Espada')

    new_card = Card(2, 'Basto')
    new_card.print()

    new_cards = [Card(11,'Basto'), Card(8,'Espada'), Card(10,'Oro')]


    toplay = []
    i = 0

    for car_hand in new_cards:
        toplay.append(compare(card_table, car_hand))

    print_hand(new_cards, toplay)

    selected = False

    print('Select one card from your hand: ')

    while selected == False:

        Ix = input()

        try:
            I = int(Ix)
            if toplay[I-1] == True:
                selected = True
                break
            else:
                selected = False
            print('Select one correct card from your hand: ')
        except:
            print('Introduce a correct number!')
            selected = False

    card_table = new_cards[I-1]
    del new_cards[I-1]



    #for car_hand in new_cards:
    #    toplay.append(compare(card_table, car_hand))

    #print_hand(new_cards, toplay)


    #compare(card_table, new_card)


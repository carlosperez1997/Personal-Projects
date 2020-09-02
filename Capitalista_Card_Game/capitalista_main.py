class Card:
    '''
    Class of a Card.
        Methods:
            - __init__(suit, number)
            - __str__()
            - compare(other)
    '''

    suit_list = ['Oro', 'Basto', 'Espada', 'Copa']
    number_list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]

    def __init__(self, suit, number):
        if number == 1 or number == 2:
            self.number = number + 12  # from 1 to 2
        else:
            self.number = number  # from 3 to 12

        self.suit = suit

    def __str__(self):
        if self.number > 12:
            name = self.suit_list[self.suit] + ' - ' + str(self.number - 12)
        else:
            name = self.suit_list[self.suit] + ' - ' + str(self.number)

        return name

    def compare(self, other):

        bool = -1
        if other.number == self.number:
            bool = 0
        elif self.number == 2 and self.suit_list(self.suit) == 'Oro':
            bool = 1
        elif other.number == 7:
            if self.number == 7 or self.number == 8:
                bool = 1
            else:
                bool = -1
        elif other.number == 10:
            if self.number <= 10:
                bool = 1
            else:
                bool = -1
        elif self.number >= other.number:
            bool = 1
        else:
            bool = -1

        return bool


class Deck:
    '''
    Class of Deck formed by cards.
        Methods:
            - __init__ (name = optional)
            - __add__card (card)
            - remove_card (card)
            - shuffle()
            - pop_card()
            - is_empty()
            - deal(hands, nCards = optional):
    '''

    def __init__(self):
        self.cards = []
        for suit in range(len(Card.suit_list)):
            for rank in range(1, len(Card.number_list) + 1):
                self.cards.append(Card(suit, rank))

    def __str__(self):
        s = ""
        for i in range(len(self.cards)):
            return str(self.cards[i]) + "\n"

        return s

    def remove_card(self, card):
        if card in self.cards:
            self.cards.remove(card)
            return True
        else:
            return False

    def shuffle(self):
        import random
        nCards = len(self.cards)
        for i in range(nCards):
            j = random.randrange(i, nCards)
            self.cards[i], self.cards[j] = self.cards[j], self.cards[i]

    def pop_card(self):
        return self.cards.pop()

    def is_empty(self):
        return (len(self.cards) == 0)

    def deal(self, hands, nCards=999):
        nHands = len(hands)
        for i in range(nCards):
            if self.is_empty():
                break
            card = self.pop_card()
            hand = hands[i % nHands]
            hand.add_card(card)


class Hand(Deck):
    '''
        Class of Hand with the different actions (methods) that a player in a card game normally does.
        It receives as an input:
            - :param Deck:
        Methods:
            - __init__ (name = optional)
            - __add__card (card)
            - __str__
            - cards_to_play(other)
    '''
    def __init__(self, name=""):
        self.cards = []
        self.name = name

    def add_card(self, card):
        self.cards.append(card)

    def __str__(self):
        s = ""
        p = 0
        for i in range(len(self.cards)):
            s = s + p * ' - ' + str(self.cards[i])
            p = 1
        return s

    def cards_to_play(self, other):
        s = ""
        p = 0
        potential_play = []

        for i in range(len(self.cards)):

            card = self.cards[i]

            if other == '':
                s = s + p * ' || ' + '(' + str(i) + ') ' + str(card) + " True "
                p = 1
                potential_play.append(1)

            elif card.compare(other) == -1:
                s = s + p * ' || ' + '(' + str(i) + ') ' + str(card) + " False "
                p = 1
                potential_play.append(card.compare(other))

            elif card.compare(other) == 1:
                s = s + p * ' || ' + '(' + str(i) + ') ' + str(card) + " True "
                p = 1
                potential_play.append(card.compare(other))

            else:
                s = s + p * ' || ' + '(' + str(i) + ') ' + str(card) + " False "
                p = 1
                potential_play.append(card.compare(other))

        return [s, potential_play]


def start_game(Deck, Hand):
    '''
    The function start_game initialises the game by asking the number of players and dealing the deck among the players.
        :param Deck: Class of Deck
        :param Hand: Class of Hand
    The output:
        :return players_hand: Hands of the players. Each hand contains several cards.
    '''
    print('------------------------ THE CAPITALISTA GAME ------------------------\n'
          'Welcome to read the rules of the game go to capitalista code below or run help(capitalista) in the terminal.')
    print(' - Select the number of players: ')
    bool = False
    while bool == False:

        input_num = input()

        try:
            num_players = int(input_num)
            if num_players > 10:
                print('Select a reasonable number of players: ')
            else:
                bool = True
                break
        except:
            print('Introduce a correct number!')
            bool = False

    if bool == True:
        deck = Deck()

        players_hand = []

        for i in range(num_players):
            name = 'Player ' + str(i)
            players_hand.append(Hand(name))

        deck.deal(players_hand)

        return players_hand


def card_election(players_hand, card_table, id_player):
    '''
    The function card_election enables the player to choose between their different cards in their hand.
    Some cards could not be played depending on the value of card in the table.
    It receives as inputs:
        :param players_hand: List of the hands of each player (type List containing Hand objects)
        :param card_table: Card on the table that the player must overcome (type Card)
        :param id_player: Index indicating the player that will play (type <int>)
    And the output variables:
        :return: A vector with:
            - potential(index): Value that indicates if the player has skipped their turn (-1), played a similar number (0) or another card that fulfills requirements (1) (type <int>)
            - card_to_play: Card selected by the player
    '''
    print('It is the turn of ', players_hand[id_player].name, ':')
    [s, potentials] = players_hand[id_player].cards_to_play(card_table)
    print(s)
    print('---------------------------------------')
    print('Choose a possible election (enter N if you want to retire from this hand):')

    bool = False

    while bool == False:
        input_num = str(input())
        if input_num == 'N' or input_num == 'n':
            return [-1, card_table]
        else:
            try:
                index = int(input_num)
                if (potentials[index] == 1 or potentials[index] == 0) and index < len(potentials):
                    card_to_play = players_hand[id_player].cards[index]
                    bool = True

                    print(' ====> ', players_hand[id_player].name, ' played ', card_to_play)

                    print('--------------------------------------')
                    print(' - Card in the Table: ', card_to_play)

                    return [potentials[index], card_to_play]
                else:
                    print('  Enter a possible index to play!')
            except:
                print('  Enter a number please')


def next_player (players_on_round, id_player_ant, increment):
    '''
    The function next_player determines the following player that will play in this round.
    Note that some players could have retired in this round so they cannot play during this round.
    It receives as inputs:
        :param players_on_round: Vector with a boolean of the players still playing on this round (type <boolean>)
        :param id_player_ant: Index of the player that was playing during this round (type <int>)
        :param increment: Number of players minus 1 that cannot play (type <int>) (see Rules of the Capitalista)
    And the output variables:
        :return id: Index of the player that will play after this play (type <int>)
    '''
    num_players = len(players_on_round)
    id = id_player_ant + increment
    id = id % num_players

    found = False

    while found == False:
        if players_on_round[id] == 0:
            found = True
        else:
            id = id + 1
            id = id % num_players

    return id


def next_play(players_hand, card_table, players_on_round, first_play=False, id_player=0):
    '''
    The function next_play computes the play of a player, by showing their hand and asking about their next decision.
    It receives the following input variables:
        :param players_hand: List of the hands of each player (type List containing Hand objects)
        :param card_table: Card on the table that the player must overcome (type Card)
        :param players_on_round: Vector with a boolean of the players still playing on this round (type <boolean>)
        :param first_play: Bool that indicates that the game is starting (type <boolean>)
        :param id_player: Index indicating the player that will play (type <int>)
    And the output variables:
        :return: a vector with:
            - id_player: Index indicating the player that will play the next play (type <int>)
            - card_selected: Card selected by the player (type Card)
            - player_continue: Bool that indicates if the player will continue playing in this round (type <boolean>)
    '''
    num_players_finished = 0
    num_players = len(players_hand)

    if first_play == True:
        [id, card_to_play] = search_card(players_hand, card_table)
        show_cards_to_players(players_hand)

        print(' ====> ', players_hand[id].name, ' played ', card_to_play)

        print('--------------------------------------------------')
        print(' - Card in the Table: ', card_table)

        card_table = card_to_play

        players_hand[id].remove_card(card_to_play)
        id_player = id + 1;
        id_player = id_player % num_players

        return [id_player, card_table, True]

    else:

        [value, card_selected] = card_election(players_hand, card_table, id_player)

        if value == 0:
            players_hand[id_player].remove_card(card_selected)
            id_player = next_player(players_on_round, id_player, 2)
            player_continue = True
        elif value == 1:
            players_hand[id_player].remove_card(card_selected)
            id_player = next_player(players_on_round, id_player, 1)
            player_continue = True
        else: # value = -1
            players_on_round[id_player] = 1
            id_player = next_player(players_on_round, id_player, 1)
            player_continue = False

        return [id_player, card_selected, player_continue]


def show_cards_to_players(players_hand):
    '''
        The show_cards_to_players prints the cards of the players and how many they have left.
        It receives the following input variables:
            :param players_hand: List of the hands of each player (type List containing Hand objects)
        It doesn't have an output variable.
        '''
    for i in range(len(players_hand)):
        cards_left = len(players_hand[i].cards)
        print(players_hand[i].name + ' - Left: ', cards_left)
        print(players_hand[i])


def search_card (players_hand, card_to_search, id=0):
    '''
    The search_card function searches for a card between the hands of the players.
    It receives the following input variables:
        :param players_hand: List of the hands of each player (type List containing Hand objects)
        :param card_to_search [suit number] Card we are looking for (type <int>)
        :param id: Index of the player (if we know which player is the holder) (type <int>)
    And the output variables:
        :return: a vector with:
                - i : Index of the player owning the card we were looking for) (type <int>)
                - card : the card we were looking for (type Card)
    '''
    if id == 0:
        for i in range(len(players_hand)):
            for card in players_hand[i].cards:
                if card_to_search.number == card.number and card_to_search.suit == card.suit:
                    return [i, card]
                    break
    else:
        for card in players_hand[id].cards:
            if card_to_search.number == card.number and card_to_search.suit == card.suit:
                return [id, card]
                break


def check_finish(players_hand, num_players_finished):
    '''
    The function check_finish checks if a player has no cards left in their hand.
    The input variables are:
        :param players_hand: List of the hands of each player (type List containing Hand objects)
        :param num_players_finished: Number of players that have finished the game
    And the output:
        :return num_players_finished: Number of players that have finished the game updated
            
    '''
    for hand in players_hand:
        if hand.is_empty() == 0:
            num_players_finished = num_players_finished + 1
            if num_players_finished == 1:
                print('\n \n THE WINNER IS', hand.name)

    return [num_players_finished]


def capitalista():
    '''
    This is the main function of the game of cards : Capitalista. The game is played if a Spanish deck.
    Spanish deck is formed by four suits: Oro, Espada, Bastos and Copa; and numbers are from 1 to 12.
    The whole game is run using the terminal.
    The rules of the game are the following:
        - Numbers ordered from smaller to bigger:
            3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - 11 - 12 - 1 - 2
        - In order to start the game, the player with the 3 of Oro starts the game.
        - After a 7, the following player must play a 7 or/and 8
        - After a 10, the following player must play a card with a score below or equal to 10.
        - The highest car is 2. And the card that works in every situation is the 2 of Oro.
        - If a player sets the same number of a card as the one in the table, the following player will be skipped but they will continue playing in this round
        - If a player cannot play or does not want to play, the player will no longer be playing in this round
        - A round is ended when only one player is playing. After a round ends all players (with cards left) will be playing again.
    :return:
    '''

    card_start = Card(0, 3)
    players_hand = start_game(Deck, Hand)

    print(' - The hands of each player are:')
    players_out_round = []
    for i in range(0, len(players_hand)):
        players_out_round.append(0)

    [id_player, card_table, player_continue] = next_play(players_hand, card_start, players_out_round , True)

    num_players_finished = 0
    num_plays = 0
    num_players_out_round = 0

    while num_players_finished != len(players_hand):
        if num_players_out_round != len(players_hand)-1:
            [id_new, card_table, player_continue] = next_play(players_hand, card_table, players_out_round, False, id_player)
            if player_continue == False:
                players_out_round[id_player] = 1
                num_players_out_round = num_players_out_round + 1
            id_player = id_new
        else:
            for i in range(0, len(players_hand)):
                players_out_round[i] = 0
            print('\n NEW ROUND: ')
            [id_new, card_table, player_continue] = next_play(players_hand, '', players_out_round, False)
        num_plays = num_plays + 1

        [num_players_finished] = check_finish(players_hand, num_players_finished)


if __name__ == '__main__':
    capitalista()



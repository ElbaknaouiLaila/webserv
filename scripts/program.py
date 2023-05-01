from __future__ import print_function


def display_number(number):
    """
    Display a number in big using characters like '#' and '$' in the terminal.
    """
    digits = {
        '0': [
            '#####',
            '#   #',
            '#   #',
            '#   #',
            '#####',
        ],
        '1': [
            '  #  ',
            ' ##  ',
            '# #  ',
            '  #  ',
            '#####',
        ],
        '2': [
            '#####',
            '    #',
            '#####',
            '#    ',
            '#####',
        ],
        '3': [
            '#####',
            '    #',
            '#####',
            '    #',
            '#####',
        ],
        '4': [
            '#   #',
            '#   #',
            '#####',
            '    #',
            '    #',
        ],
        '5': [
            '#####',
            '#    ',
            '#####',
            '    #',
            '#####',
        ],
        '6': [
            '#####',
            '#    ',
            '#####',
            '#   #',
            '#####',
        ],
        '7': [
            '#####',
            '    #',
            '   # ',
            '  #  ',
            '  #  ',
        ],
        '8': [
            '#####',
            '#   #',
            '#####',
            '#   #',
            '#####',
        ],
        '9': [
            '#####',
            '#   #',
            '#####',
            '    #',
            '#####',
        ],
    }

    # Convert the number to a string
    number_str = str(number)

    # Loop through each digit in the number and print the corresponding big representation
    for row in range(5):
        for digit in number_str:
            print(digits[digit][row], end=' ')
        print()


# Call the function to display the number 1337
display_number(20020407)
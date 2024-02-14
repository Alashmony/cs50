#include <cs50.h>
#include <math.h>
#include <stdio.h>

int len_number(long number);
int first_numbers(long number);
int get_located(long number, int location);
int sum_algo(long number);
string validate_card(long number);

int main(void)
{
    long card_n = get_long("Number: ");
    // int card_len = len_number(card_n);
    // printf("Card length is %i\n", card_len);
    // int first_num = first_numbers(card_n);
    // printf("First numbers are %i\n", first_num);

    string validation_result = validate_card(card_n);
    printf("%s", validation_result);
}

string validate_card(long number)
{
    string validation;
    // Check the length
    int len = len_number(number);
    // printf("%i\n",len);
    // get first two numbers
    int first_numbers = number / powl(10, len - 2);
    // printf("%i\n",first_numbers);
    int card_sum = sum_algo(number);
    int luhn_validator = get_located(card_sum, 1);
    // printf("%i\n",luhn_validator );

    if (luhn_validator == 0 && len >= 13 && len <= 16)
    {
        if (len == 15 && (first_numbers == 34 || first_numbers == 37))
        {
            validation = "AMEX\n";
        }
        else if (len == 16 &&
                 (first_numbers == 51 || first_numbers == 52 || first_numbers == 53 || first_numbers == 54 || first_numbers == 55))
        {
            validation = "MASTERCARD\n";
        }
        else if ((len == 13 || len == 16) && (first_numbers >= 40 && first_numbers < 50))
        {
            validation = "VISA\n";
        }
        else
        {
            validation = "INVALID\n";
        }
    }
    else
    {
        validation = "INVALID\n";
    }

    return validation;
}

int len_number(long number)
{
    // Count the length of the card
    int len;
    for (len = 0; number > (long) 0; len++)
    {
        number = number / 10;
    }
    return len;
}

int first_numbers(long number)
{
    // Get first two numbers

    int length = len_number(number) - 2;
    long denom = powl(10, length);

    int first_numbers = number / denom;

    return first_numbers;
}

int get_located(long number, int location)
{
    // gets a number using its location within the card number
    long denom = powl(10, location);
    long rem_denom = powl(10, location - 1);
    long remainder = number % denom;
    // printf("remainder is %i\n",remainder);
    int wanted_num = remainder / rem_denom;
    // printf("required number is %i\n", wanted_num);
    return wanted_num;
}

int sum_algo(long number)
{
    int sum = 0;
    // sum first_number + 2 * second ... etc
    int length = len_number(number);

    // Iterate over each number of the card
    for (int i = 1; i < length + 1; i++)
    {
        int num = get_located(number, i);

        // Checks if the location is even or odd
        if (i % 2 != 0)
        {
            sum += num;
        }
        else
        {
            int d_num = num * 2;

            // checks if the number is greater than 10 (two fields should be summed)
            if (d_num < 10)
            {
                sum += d_num;
            }
            else
            {
                for (int cell = 1; cell < 3; cell++)
                {
                    sum += get_located(d_num, cell);
                }
            }
        }
        // printf("%i: Number is %i and the total is %i\n",i, num,sum);
    }
    return sum;
}

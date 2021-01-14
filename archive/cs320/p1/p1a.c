/**
 * Program 1a
 * An ANSI C program to describe time to pay off loan (months)
 * and amount of interest paid over the life of the loan.
 * CS320
 * 9 September 2020
 * @author Brandon Nguyen cssc2650
 */
#include <stdio.h>

void printId();
void getLoanFactors(double *loanAmount, double *intRate, double *payment);
int getMonths(double loanAmt, double intRate, double payment, double *totalInt);

void printId() {
    printf("\nProgram 1, cssc2650, Brandon Nguyen\n\n");
}

void getLoanFactors(double *loanAmount, double *intRate, double *payment) {
    scanf("%lf", loanAmount);
    scanf("%lf", intRate);
    scanf("%lf", payment);
    printf("$%7.2lf Loan amount\n", *loanAmount);
    printf("%8.2lf Annual interest rate\n", *intRate);
    printf("$%7.2lf Monthly Payment\n\n", *payment);
}

int getMonths(double loanAmt, double intRate, double payment, double *totalInt) {
    int months = 0;
    printf(" LoanAmt  Month TotalInt\n");
    while(loanAmt > 0) {
        double currentInterest = (loanAmt * intRate) / 12;
        loanAmt -= payment;
        loanAmt += currentInterest;
        *totalInt += currentInterest;
        months += 1;
        printf(" %7.2lf  %5d %8.2lf\n", loanAmt, months, *totalInt);
    }
    return months;
}

int main() {
    double loanAmt, /* starting amount of the loan in dollars */
           intRate, /* annual interest rate between 0 and 1.0 */
           payment, /* monthly payment made toward the lone in dollars */
           totalInt; /* total amount of interest in dollars paid over life of loan */
    printId();
    getLoanFactors(&loanAmt, &intRate, &payment);
    int months = getMonths(loanAmt, intRate, payment, &totalInt);
    /* pretty print results */
    printf("\n%d months to pay off loan\n", months);
    printf("$%.2lf total interest paid\n", totalInt);
    return 0;
}

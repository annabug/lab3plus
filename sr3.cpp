//Zhuk Anna            Lab3 srom var#7
#include <iostream>
#include <string>
#include <stdlib.h>//strtol()
//outFile (todo)
//getStringPol() (todo)

//p(x) = mod[251] = {};   p(x) = mod[0] = mod[1] = mod[4] = mod[14] = mod[251] = 1;
//502/2 = 251; "x^0" -> polynom[0],  "x^1" -> polynom[1], ... , "x^251" - > polynom[251]

//neutral for "+" is : int NullPolynom[502] = {};
//neutral for "*" is : int Polynom[502] = {}; Polynom[0] = 1;
using namespace std;



void setPolynomNull(int* op) // polynom = 0;
{
    for(int i = 0; i < 502; i++)
    {
        op[i] = 0;
    }
}

void setPolynomStr(int* op, string str) // string >> polynom
{
    setPolynomNull(op);
    string temp;
    int k = 0;
    for(int i = str.length() -1 ; i >= 0; i--)
    {
        temp = str[i];
        op[k] = strtol(temp.c_str(), nullptr, 2);
        k++;
    }
}



void setPolynomPol(int* leftOp, int* rightOp) // operator = ; for case (polynomLeft = polynomRight)
{
    for(int i = 0; i < 502; i++)
    {
        leftOp[i] = rightOp[i];
    }
}

int degreePolynom(int* op)// works properly
{
    for(int i = 501; i > -1; i-- )
    {
        if(op[i] != 0)
        {
            return i;
        }
    }
}

void getPolynom(int* op) // cout << polynom
{
    for(int i = degreePolynom(op); i > -1; i--)
    {
        cout << op[i];
        //cout << op[i] << "*x^" << i ;
    }
    cout << endl;
}

void additionPol(int* result, int* operand1, int* operand2) // polynom = poly1 + poly2
{
    for(int i = 0; i < 502; ++i)
    {
        result[i] = operand1[i]^operand2[i];
    }
}

void shiftRightPol(int* op, int shift) //poly = poly >> shift
{
    int temp[502] = {};
    for(int i = shift; i < 502; i++)
    {
        temp[i] = op[i-shift];
    }
     setPolynomPol(op, temp);
}

void reduction(int* result) // polynom = polynom(mod p(x))
{
    int mod[502] = {};
    mod[0] = mod[1] = mod[4] = mod[14] = mod[251] = 1;


    int temp[502] = {};
    int operand[502] = {};
    setPolynomPol(operand, result);
    int degree = degreePolynom(operand);
    while(degree >= degreePolynom(mod))
    {
        setPolynomPol(temp, mod);
        shiftRightPol(temp, (degreePolynom(operand) - degreePolynom(mod)));

        additionPol(operand, operand, temp);
        degree = degreePolynom(operand);

    }
    setPolynomPol(result, operand);

}

void multiplicationPol(int* result, int* operand1, int* operand2)// result = poly1 * poly2
{
    int temp[502] = {};
    int uncertainMoment[502] = {}; // temporary massive for correct work of case : multiplicationPol(poly1, poly1, poly2);
    //setPolynomPol(uncertainMoment, operand1);
    for(int i = 0; i < 502; i++)
    {
        if(operand2[i]==1) // (operand2[i])
        {
            setPolynomPol(temp, operand1);
            shiftRightPol(temp, i);
            additionPol(uncertainMoment, uncertainMoment, temp);
        }
    }
    setPolynomPol(result, uncertainMoment);
    reduction(result);//, result, result);
}


void squarePol(int* result, int* operand1)
{
    int temp[502] = {};
    int uncertainMoment[502] = {}; //for correct answer in sutuation squarePol(poly, poly);

    for(int i = 0; i < 502; i++)
    {
        if(operand1[i]==1) // (operand2[i])
        {
            setPolynomPol(temp, operand1);
            shiftRightPol(temp, i);
            additionPol(uncertainMoment, uncertainMoment, temp);
        }
    }
    setPolynomPol(result, uncertainMoment);
    reduction(result);//, result, result);
}

void powerPol(int* result, int* operand1, int* operand2) // result = pow(operand1, n) ; operand2 = n
{
    int uncertainMoment[502] = {}; // this is not clear "uncertainMoment" it is "temp"
    uncertainMoment[0] = 1;
    int tempOp[502] = {};
    setPolynomPol(tempOp, operand1);

    //operand1 is saved
    for(int i = 0; i < 251; i++)
    {
        if(operand2[i] == 1)
        {
            multiplicationPol(uncertainMoment, tempOp, uncertainMoment);
            reduction(uncertainMoment);
        }
        squarePol(tempOp, tempOp); //  it is safe
        reduction(tempOp);
    }
    setPolynomPol(result, uncertainMoment);
}

void reversePol(int* op)
{
    int result[502] = {};
    result[0] = 1;
    for(int i = 0; i < 250; i++)
    {
        multiplicationPol(result, result, op);
        squarePol(op, op);
    }
    squarePol(result, result);
    reduction(result);
    setPolynomPol(op, result);
}

void slidPol(int* op)
{
    int tempOp1[502] = {};
    int tempOp2[502] = {};
    setPolynomPol(tempOp1, op);
    setPolynomPol(tempOp2, op);


    for(int i = 0; i < 251; i++)
    {
        additionPol(tempOp1, tempOp1, tempOp2);
        squarePol(tempOp2, tempOp2);
    }
    setPolynomPol(op, tempOp1);

}


int main()
{
    int poly1[502] = {};
    int poly2[502] = {};
    int resultPol[502] = {};
    int nPol[502] = {};

    string initStr = "11000000001001101101000100100100100101101001011010101101001100100011011100100000011101010010100010011000001110011110010111001111101010111101010101111100101100100111001100111010001011001011000011011000101001101110100011110110010101010010110110001000100";
    setPolynomStr(poly1, initStr);

    initStr = "10111001101100010011110111110011000100010001010010110011111001000000001000001010110111000010011101010011001111111001111010001000111100111101110011011001111100101010001110101111011101111110011010000000110010010101001001100010010110000000101001101101101";
    setPolynomStr(poly2, initStr);

    initStr = "11001001111111011010001000010001100101000010100001011111100110111010100011010110100010100001010001011001011010110111011010001010111011101111100011101110100011100110101011010010111111001110101010011110001100011110110011101011010110001100010101110101110";
    setPolynomStr(nPol, initStr);


    getPolynom(poly1);
    getPolynom(poly2);
    getPolynom(nPol);
    getPolynom(resultPol);
    cout << endl << endl;

    //+
//    additionPol(resultPol, poly1, poly2);
//    getPolynom(resultPol);

    //*
//    multiplicationPol(resultPol, poly1, poly2);
//    getPolynom(resultPol);
//    cout << endl;
//    multiplicationPol(poly1, poly1, poly2);//is't correct
//    getPolynom(poly1);


    //pow 2
//    squarePol(resultPol, poly1);
//    getPolynom(resultPol);
//cout << endl;
//    squarePol(poly1, poly1);
//    getPolynom(poly1);

    //pow ( max stepin' 2^m-1) ne bol'she 252 bitov
//    powerPol(resultPol, poly1, nPol);
//    getPolynom(resultPol);

    //pol^(-1)
//    reversePol(poly1);
//    getPolynom(poly1);
//    reversePol(poly1);

    //slid
//    slidPol(poly1);
//    getPolynom(poly1);


    cout << endl << endl;
    getPolynom(poly1);
    getPolynom(poly2);
    getPolynom(nPol);
    getPolynom(resultPol);


    return 0;
}

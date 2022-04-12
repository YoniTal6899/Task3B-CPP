#include "Matrix.hpp"
#include <stdexcept>

using namespace std;
using namespace zich;

Matrix::Matrix(const vector<double>& DATA, const int r, const int c)
{
    if (r <= 0 || c <= 0){throw invalid_argument("Matrix rows and columns must be positive!");}
    if (r * c != DATA.size()){throw invalid_argument("(Rows x Columns) do not match the size!");}
    data = vector<double>(DATA);
    rows = (unsigned int)r; 
    cols = (unsigned int)c; 
}
double zich::sum(const Matrix& a)
{
    double sum = 0;
    for (unsigned int i = 0; i < a.data.size(); i++)
    {
        sum += a.data.at(i);
    }
    return sum;
}
void Matrix::check_sizes(const Matrix& a) const
{
    if (rows != a.rows || cols != a.cols){throw runtime_error("Matrices are not the same size!");}
}
Matrix Matrix::operator+() const //Unary + nothing to do
{
    return *this;
}
Matrix Matrix::operator-() const // Unary - multyply every element by -1
{
    Matrix ans = Matrix(*this);
    for (unsigned int i = 0; i < ans.data.size(); i++)
    {
        ans.data.at(i) *= -1;
    }
    return ans;
}
Matrix& Matrix::operator++() //prefix
{
    *this += 1;
    return *this;
} 
Matrix& Matrix::operator--() //prefix
{
    *this -= 1;
    return *this;
}
Matrix Matrix::operator++(int) //postfix
{
    Matrix ans = *this;
    *this += 1;
    return ans;
}
Matrix Matrix::operator--(int) //postfix
{
    Matrix ans = *this;
    *this -= 1;
    return ans;
}
Matrix Matrix::operator+(const double n) const
{
    Matrix ans = *this;
    for (unsigned int i = 0; i < ans.data.size(); i++)
    {
        ans.data.at(i) += n;
    }
    return ans;
}
Matrix Matrix::operator-(const double n) const
{
    Matrix ans = *this;
    for (unsigned int i = 0; i < ans.data.size(); i++)
    {
        ans.data.at(i) -= n;
    }
    return ans;
}
Matrix Matrix::operator*(const double n) const
{
    Matrix ans = *this;
    for (unsigned int i = 0; i < ans.data.size(); i++)
    {
        ans.data.at(i) *= n;
    }
    return ans;
}
Matrix Matrix::operator+(const Matrix& a) const
{
    check_sizes(a);
    Matrix ans = *this;
    for (unsigned int i = 0; i < ans.data.size(); i++)
    {
        ans.data.at(i) += a.data.at(i);
    }
    return ans;
}
Matrix Matrix::operator-(const Matrix& a) const
{
    check_sizes(a);
    Matrix ans = *this;
    for (unsigned int i = 0; i < ans.data.size(); i++)
    {
        ans.data.at(i) -= a.data.at(i);
    }
    return ans;
}
Matrix Matrix::operator*(const Matrix& a) const
{    
    if (cols != a.rows){throw invalid_argument("Unmatching sizes!");}    
    unsigned int nr = rows;
    unsigned int nc = a.cols;
    vector<double> nd(nr * nc, 0); 
    for (unsigned int i = 0; i < nr; i++)
    {
        for (unsigned int j = 0; j < nc; j++)
        {
            for (unsigned int p = 0; p < cols; p++)
            {
                nd.at(i * nc + j) +=data.at(i * cols + p) * a.data.at(p * a.cols + j); //For 1D array we use the formula: mat[i, j] = arr[i*cols + j];
            }
        }
    }
    return Matrix(nd, (int)nr, (int)nc);
}
Matrix& Matrix::operator+=(const double n)
{
    *this = *this + n;
    return *this;
}
Matrix& Matrix::operator-=(const double n)
{
    *this = *this - n;
    return *this;
}
Matrix& Matrix::operator*=(const double n)
{
    *this = *this * n;
    return *this;
}
Matrix& Matrix::operator+=(const Matrix& a)
{
    *this = *this + a;
    return *this;
}
Matrix& Matrix::operator-=(const Matrix& a)
{
    *this = *this - a;
    return *this;
}
Matrix& Matrix::operator*=(const Matrix& a)
{
    *this = *this * a;
    return *this;
}
Matrix& Matrix::operator=(const Matrix& a) //copy constructor
{
    data = vector<double>(a.data);
    rows = a.rows;
    cols = a.cols;
    return *this;
}
bool Matrix::operator==(const Matrix& a) const
{
    check_sizes(a);
    for (unsigned int i = 0; i < data.size(); i++){if (data.at(i) != a.data.at(i)){return false;}}
    return true;
}
bool Matrix::operator!=(const Matrix& a) const
{
    return !(*this==a);
}
bool Matrix::operator>(const Matrix& a) const
{
    check_sizes(a);
    return sum(*this) > sum(a);
}
bool Matrix::operator<(const Matrix& a) const
{
    check_sizes(a);
    return sum(*this) < sum(a);
}
bool Matrix::operator>=(const Matrix& a) const
{
    check_sizes(a);
    return sum(*this) >= sum(a);
}
bool Matrix::operator<=(const Matrix& a) const
{
    check_sizes(a);
    return sum(*this) <= sum(a);
}
ostream& zich::operator<<(ostream& os, const Matrix& a)
{
    for (unsigned int i = 0; i < a.rows; i++)
    {
        os << "["; //start each row with '['
        for (unsigned int j = 0; j < a.cols; j++)
        {
            os << a.data.at(i * a.cols + j);
            if (j < a.cols - 1) //print ' ' while the value is not the last in the row
            { 
                os << " ";
            }
        }
        os << "]"; //end each row with ']'
        if (i < a.rows - 1) //print '\n' while we aren't in the end of the last row
        {
            os << "\n";
        }
    }
    return os;
}
istream& zich::operator>>(istream& is, Matrix& a){
    string input;
    getline(is, input);
    unsigned int cols = 0;
    unsigned int rows = 0;
    unsigned int curr_c = 0;
    bool iteration_1 = true;
    unsigned int i = 0;
    vector<double> values = {};
    while (i < input.length() - 1)
    {
        //check if each row must start with "["
        if (input.at(i) != '['){throw invalid_argument("Please follow this pattern: \'[<n1> <n2> <n3> <...> <nk>], [...]\'.");}
        while (input.at(i++) != ']')// until the end of the row
        {
            string number;
            while (input.at(i) != ' ' && input.at(i) != ']')
            {
                number += input.at(i++);
            }
            try{values.push_back(stod(number));} //convert number to double and add it to the vector
            catch (invalid_argument e){throw invalid_argument("Make sure there are no spare blank spaces and/or special characters");}
            cols++;
        }
        if (iteration_1) //set the number of cols
        { 
            curr_c = cols;
            iteration_1 = false;
        }
        //if there is an inconsistent number of columns
        else if (cols != curr_c){throw invalid_argument("Inconsistent number of values per row!");}
        rows++;
        cols = 0;
        if (i < input.length() - 1) //if we haven't reached the end of the string it means there are more rows
        {
            //each row must be seperated by a ',', then ' ' 
            if (input.at(i) != ',' || input.at(i+1) != ' '){throw invalid_argument("Please follow this pattern: \'[<n1> <n2> <n3> <...> <nk>], [...]\'.");}
            i += 2;
        }
    }
    a = Matrix(values, (int)rows, (int)curr_c); //create the new matrix
    return is;
}
Matrix zich::operator+(const double n, Matrix& a)
{
    return a + n;
}
Matrix zich::operator-(const double n, Matrix& a)
{
    return -(a - n);
}
Matrix zich::operator*(const double n, Matrix& a)
{
    return a * n;
}
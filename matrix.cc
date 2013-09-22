#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template<size_t H, size_t W, typename T = float>
class Matrix
{

public:

    Matrix(initializer_list<initializer_list<T>> l)
           : m_m(l.begin(), l.end())
    {
    }

           Matrix(const Matrix & n)
           : m_m(n.m_m) {}

    // Alkeisrivitoimitukset
                    
    Matrix<H,W,T> swap_rows(size_t r, size_t s) const
    {
        Matrix<H,W,T> ans(*this);
        swap(ans.m_m[r], ans.m_m[s]);
        return ans;
    }

    Matrix<H,W,T> mul_row(size_t r, T k) const
    {
        Matrix<H,W,T> ans(*this);
        transform(ans.m_m[r].begin(),
                  ans.m_m[r].end(),
                  ans.m_m[r].begin(),
                  [=](T t) { return t*k; });
        return ans;
    }

    Matrix<H,W,T> add_row(size_t r, size_t s, T k = T(1)) const
    {
        Matrix<H,W,T> ans(*this);
        for(size_t x = 0; x < W; ++x)
            ans.m_m[r][x] += k*this->m_m[s][x];
        return ans;
    }
        

private:
           
    vector<vector<T>> m_m;

    public:
           template<size_t H2, size_t W2, typename T2>
    friend ostream & operator<<(ostream & os, const Matrix<H2,W2,T2> & m);

};
    
template<size_t H, size_t W, typename T>
ostream & operator<<(ostream & os, const Matrix<H,W,T> & m)
{
    for(size_t x = 0; x < H; ++x)
    {
        for(size_t y = 0; y < W; ++y)
        {
            os << m.m_m[x][y] << ' ';
        }
        os << endl;
    }

    return os;
}
 
int main()
{
    Matrix<3,3,float> m {{0.0, 3.4, 12.3},
        {0.0, 0.0, 1.0}, {2.0, 3.141, 2.71}};
    cout << m << "---------------" << endl;
    m = m.swap_rows(0, 1);
    cout << m << "---------------" << endl;
    m = m.mul_row(2,2.0);
    cout << m << "---------------" << endl;
    m = m.add_row(0,1,2);
    cout << m << "---------------" << endl;
}

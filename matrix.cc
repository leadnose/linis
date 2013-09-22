#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template<size_t H, size_t W, typename T = float>
class Matrix
{

public:

    Matrix(initializer_list<initializer_list<T>> l)
     : m_m(l.begin(), l.end()) {}

    Matrix(const Matrix & n)
     : m_m(n.m_m) {}

    // Alkeisrivitoimitukset
                    
    Matrix<H,W,T> swap_rows(size_t r, size_t s) const
    {
        cout << *this << "-- R" << r+1 << " <->  R" << s+1 << " -->" << endl;
        Matrix<H,W,T> ans(*this);
        swap(ans.m_m[r], ans.m_m[s]);
        cout << ans << endl;
        return ans;
    }

    Matrix<H,W,T> mul_row(size_t r, T k) const
    {
        cout << *this << "-- " << k << "R" << r+1 << " -->" << endl;
        Matrix<H,W,T> ans(*this);
        transform(ans.m_m[r].begin(),
                  ans.m_m[r].end(),
                  ans.m_m[r].begin(),
                  [=](T t) { return t*k; });
        cout << ans << endl;
        return ans;
    }

    Matrix<H,W,T> add_row(size_t r, size_t s, T k = T(1)) const
    {
        if(k == 1)
            cout << *this << " -- R" << r+1 << " + " <<  "R" << s+1 << " -->" << endl;
        else if(k == -1)
            cout << *this << " -- R" << r+1 << " - " <<  "R" << s+1 << " -->" << endl;
        else if(k < 0)
            cout << *this << " -- R" << r+1 << " - " << -k <<  "R" << s+1 << " -->" << endl;
        else
            cout << *this << " -- R" << r+1 << " + " << k <<  "R" << s+1 << " -->" << endl;
        Matrix<H,W,T> ans(*this);
        for(size_t x = 0; x < W; ++x)
            ans.m_m[r][x] += k*this->m_m[s][x];
        cout << ans << endl;
        return ans;
    }

    Matrix<H,W,T> nollarivit_alas() const
    {
        Matrix<H,W,T> ans(*this);

        auto on_nollarivi = [](vector<T> r) { return r.end() == find_if(r.begin(), r.end(), [](T t) { return t != 0; }); };
        size_t swap_rivi = H-1;
        
        for(size_t row = 0; row < H-1; ++row)
        {
            if(on_nollarivi(ans.m_m[row]))
            {
                ans = ans.swap_rows(row,swap_rivi);
                swap_rivi--;
            }
            
        }

        return ans;
    }
    

    /**
       - Mahdolliset nollarivit alimpina
       - kullakin rivillä ensimmäinen nollasta poikkeava alkio, johtava alkio, on ylemmän
         rivin johtavan alkion oikealla puolella
     */
    Matrix<H,W,T> porrasmatriisi() const
    {
        Matrix<H,W,T> ans(*this);
        
        ans = ans.nollarivit_alas();

        // Etsitään kustakin sarakkeesta johtava alkio, ja nollataan muitten rivien sama sarake
        // jos tarvitsee
        {
            for(size_t col = 0; col < W; ++col)
            {
                for(size_t row = col; row < H-1; ++row)
                {
                    if(ans.m_m[row][col] != 0)
                    {
                        T johtava = ans.m_m[row][col];
                        for(size_t row2 = row+1; row2 < H; ++row2)
                        {
                            if(ans.m_m[row2][col] != 0) 
                            {
                                T joo = ans.m_m[row2][col];
                                T kerroin = -joo/johtava;
                                //cout << "LEL: " << endl << ans << endl;
                                ans = ans.add_row(row2, row, kerroin);
                                //cout << "LOL: " << endl << ans << endl;
                            }
                        }
                        break;
                    }
                }
            }
        }
        
        return ans;
    }

    Matrix<H,W,T> redusoitu() const
    {
        Matrix<H,W,T> ans(*this);

        // Johtavien Kertoimet ykkösiksi
        
        for(size_t row = 0; row < H; ++row)
        {
            for(size_t col = 0; col < W; ++col)
            {
                if(ans.m_m[row][col] != 0)
                {
                    T johtava = ans.m_m[row][col];
                    if(johtava != 1)
                    {
                        T kerroin = 1/johtava;
                        ans = ans.mul_row(row, kerroin);
                    }
                    break;
                }
            }
        }

        // Muut rivit nolliksi

        size_t max_idx = W;

        for(size_t row = H-1; row > 0; row--)
        {
            // Etsi johtava indeksi
            for(size_t i = 0; i < max_idx; i++)
            {
                if(ans.m_m[row][i] != 0)
                {
                    max_idx = i;
                    T johtava = ans.m_m[row][i];
                    for(int row2 = row-1; row2 >= 0; row2--)
                    {
                        T juttu = ans.m_m[row2][i];
                        if(juttu != 0)
                        {
                            T kerroin = juttu / johtava;
                            ans = ans.add_row(row2, row, -kerroin);
                        }
                    }

                    break;
                }
            }
        }

        return ans.nollarivit_alas();

    }

    

    vector<T> operator[](size_t i) const
    {
        return m_m[i];
    }
        

private:
           
    vector<vector<T>> m_m;

};
    
template<size_t H, size_t W, typename T>
ostream & operator<<(ostream & os, const Matrix<H,W,T> & m)
{
    for(size_t row = 0; row < H; ++row)
    {
        for(size_t col = 0; col < W; ++col)
        {
            T tmp = m[row][col];
            if(tmp == -0)
                tmp = 0;
            os << tmp << ' ';
        }
        os << endl;
    }

    return os;
}
 
int main()
{
    Matrix<4,4,float> m 
    {{ 2, 1, 4, 3},
        {0, 2, -4, 1},
        {1, 0, 3, 0},
            {4, 0, 12, 2}};
            
    m = m.porrasmatriisi();
    cout << m << "---------------" << endl;

    m = m.redusoitu();
    cout << m << "---------------" << endl;
    
}

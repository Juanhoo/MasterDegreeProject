
#include <iostream>
#include <vector>
#include <deque>
#include <chrono>
#include <random>
#include <memory>
#include <fstream>
#include <algorithm>    // std::shuffle
#include <set>

#define debug(x) std::cerr << __LINE__ << " " << #x << " == " << (x) << std::endl

struct triangle_number
{
    double m_core;
    double m_half_support;

	public:
	triangle_number ()
	{
		m_core = 0; 
		m_half_support = 0;
	}
	triangle_number (const double core, const double half_support) : m_core (core), m_half_support(half_support) {}

	triangle_number(const triangle_number & t) = default;
};

std::ostream & operator<< (std::ostream & ss, const triangle_number & t)
{
	ss << t.m_core << " " << t.m_half_support;
	return ss;
}

triangle_number operator+(const triangle_number& first, const triangle_number& second) 
{
    return triangle_number{ first.m_core + second.m_core, std::fmax(first.m_half_support, second.m_half_support) };
}

triangle_number & operator+=(triangle_number& first, const triangle_number& second) 
{
	first.m_core += second.m_core;
	first.m_half_support = std::fmax(first.m_half_support, second.m_half_support);

	return first;
}

triangle_number operator-(const triangle_number& first, const triangle_number& second) 
{
    return triangle_number{ first.m_core - second.m_core, std::fmax(first.m_half_support, second.m_half_support) };
}

triangle_number operator*(const triangle_number& first, const triangle_number& second) 
{
    return triangle_number{ first.m_core * second.m_core, std::fmax(first.m_half_support, second.m_half_support) };
}

triangle_number square(const triangle_number& first)
{
    return first * first;
}

triangle_number sqrt(const triangle_number& first) 
{
    return triangle_number{ std::sqrt(first.m_core) , first.m_half_support };
}


class punkt 
{
	public: 
		std::size_t id;
		std::string etykieta;
		std::vector<triangle_number> wspolrzedne;
}; 

class zakres
{
	public:
		double minimum;
		double maksimum;
};

class rozklad
{
	public:
		virtual double getvalue(std::default_random_engine & silnik) = 0;
		
		virtual ~rozklad() {};
};

class rozklad_jednorodny : public rozklad
{
	std::uniform_real_distribution<double> rozklad;

	public: 
	rozklad_jednorodny (const double minimum, const double maksimum)
	{
		rozklad = std::uniform_real_distribution<double>(minimum, maksimum);
	}
	public: 
	virtual double getvalue(std::default_random_engine & silnik) 
	{
		return rozklad(silnik);
	}
	
public:
	virtual ~rozklad_jednorodny(){};
};

class rozklad_normalny : public rozklad
{
	std::normal_distribution<double> rozklad;

	public: 
	rozklad_normalny (const double srednia, const double odchylenie)
	{
		rozklad = std::normal_distribution<double>(srednia, odchylenie);
	}
	public: 
	virtual double getvalue(std::default_random_engine & silnik) 
	{
		return rozklad(silnik);
	}
	public:
	virtual ~rozklad_normalny(){};
};


class generator
{
	protected:
		std::size_t _ile_punktow;
		std::default_random_engine silnik;
		std::vector<std::pair<std::shared_ptr<rozklad>,std::shared_ptr<rozklad>>> _rozklady;
		

	public: 
		virtual punkt tworz_punkt()
		{
			const double EPSILON { 0.01 }; // minimalne rozmycie
			punkt p;
			for (auto & r : _rozklady)
			{
				double wartosc = r.first->getvalue(silnik);
				double rozmycie = r.second->getvalue(silnik);
				p.wspolrzedne.push_back({wartosc, std::max(rozmycie, EPSILON)});
			}
			return p;
		}

	public: 
		generator (const std::vector<std::pair<std::shared_ptr<rozklad>,std::shared_ptr<rozklad>>> & rozklady) : _rozklady(rozklady)
	{
		silnik.seed(std::chrono::system_clock::now().time_since_epoch().count());
	}

		virtual std::vector<punkt> tworz_punkty(const std::size_t ile_punktow) 
		{
			std::vector<punkt> zbior;
			for (std::size_t i = 0; i < ile_punktow; i++)
				zbior.push_back(tworz_punkt());
			return zbior;
		}
};

std::ostream & operator<< (std::ostream & s, const punkt & p)
{
	if (not p.etykieta.empty())
		s << p.etykieta << ' ';
	for (const auto x : p.wspolrzedne)
		s << x << ' ';
	return s;
}

void zapisz(const std::vector<punkt> & punkty, const std::string & nazwa)
{
	std::ofstream plik (nazwa);
	if (plik)
	{
		for (const auto & p : punkty)
			plik << p << std::endl;
	}
}

void dodaj_unikalne_id (std::vector<punkt> & punkty)
{
	std::size_t id = 1; 
	for (auto & p : punkty)
		p.id = id++;
} 

void dodaj_te_sama_etykiete (std::vector<punkt> & punkty, const std::string & etykieta)
{
	std::size_t id = 1; 
	for (auto & p : punkty)
		p.etykieta = etykieta;
} 

std::vector<std::string> generuj_nazwy(const int dlugosc, const std::size_t ile_generowac)
{
	int d = dlugosc;
	d = std::max (d, 2); 
	d = std::min (d, 'z' - 'a' + 1);

    std::vector<char> znaki ('z' - 'a' + 1);
    std::iota(znaki.begin(), znaki.end(), 'a');
	std::default_random_engine silnik (std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(znaki.begin(), znaki.end(), silnik);

	std::string ziarno;
	for (int i = 0; i < d; i++)
		ziarno.push_back(znaki[i]);

	std::vector<std::string> nazwy;

	for (std::size_t i = 0; i < ile_generowac; i++)
	{
		std::next_permutation(ziarno.begin(), ziarno.end());
		//std::reverse(ziarno.begin(), ziarno.end());
		nazwy.push_back(ziarno);
	}

	for (auto & s : nazwy)
		std::reverse(s.begin(), s.end());

	return nazwy;	
}

triangle_number euklides (const punkt & l, const punkt & p)
{
	triangle_number suma;
	triangle_number roznica;
	auto size = l.wspolrzedne.size();
	for (std::size_t i = 0; i < size; i++)
	{
		roznica = l.wspolrzedne[i] - p.wspolrzedne[i];
		suma += roznica * roznica;
	}
	 
	return sqrt(suma);
}

std::unordered_map<std::string, std::unordered_map<std::string, triangle_number>> wylicz_odleglosci(const std::vector<punkt>& punkty)
{
    std::unordered_map<std::string, std::unordered_map<std::string, triangle_number>> 
	dd; // macierz odleglosci

    auto size = punkty.size();
	 
    for (std::size_t i = 0; i < size; i++)
    {
        for (std::size_t j = i + 1; j < size; j++)
        {
            auto d = euklides(punkty[i], punkty[j]);
            //auto et_i = "[" + std::to_string(i) + "]";
            //auto et_j = "[" + std::to_string(j) + "]";
            auto et_i = std::to_string(i);
            auto et_j = std::to_string(j);
            
			dd[et_i][et_j] = d;
            dd[et_j][et_i] = d;
        }
    }

    return dd;
}

void zapisz(const std::unordered_map<std::string, std::unordered_map<std::string, triangle_number>> & odleglosci, const std::string & plik)
{
	std::ofstream f (plik);
	if (f)
	{
		for (const auto [et_i, et_j_odleglosc] : odleglosci)
		{
			for (const auto [et_j, odleglosc] : et_j_odleglosc)
				f << et_i << '\t' << et_j << '\t' << odleglosc << std::endl; 
		}
		f.close();
	}
}

void generuj_dane_dla_single_complete_link(const std::string & plik_punkty,  const std::string & plik_odleglosci, const unsigned int liczba_punktow)
{
	 
// 	const unsigned int wymiary {3};
	const double srednia_polozenia  = 50;
	const double rozmycie_polozenia = 10;	

	const double srednia_rozmycia = 1;
	const double rozmycie_rozmycie = 0.05;
	 

	// tworzenie pliku treningowego
	auto pD = std::make_shared<rozklad_normalny> (rozklad_normalny(srednia_polozenia, rozmycie_polozenia));

	auto pG = std::make_shared<rozklad_normalny> (rozklad_normalny(-srednia_polozenia, rozmycie_polozenia));

	auto pR = std::make_shared<rozklad_normalny> (rozklad_normalny(srednia_rozmycia, rozmycie_rozmycie)); 
	
	std::vector<
		std::vector<std::pair<std::shared_ptr<rozklad>,std::shared_ptr<rozklad>>>> rozklady 
		{ 
			{{pD, pR}, {pD, pR}, {pD, pR}}, 
			{{pD, pR}, {pD, pR}, {pG, pR}},
			{{pD, pR}, {pG, pR}, {pD, pR}},
			{{pD, pR}, {pG, pR}, {pG, pR}},
			{{pG, pR}, {pD, pR}, {pD, pR}},
			{{pG, pR}, {pD, pR}, {pG, pR}},
			{{pG, pR}, {pG, pR}, {pD, pR}},
			{{pG, pR}, {pG, pR}, {pG, pR}}
		};

	std::vector<punkt> punkty_train;
	for (const auto & r : rozklady)
	{
		generator tworca(r);
		auto pkt = tworca.tworz_punkty(liczba_punktow / rozklady.size());
		punkty_train.insert(punkty_train.end(), pkt.begin(), pkt.end());
	}

	dodaj_unikalne_id(punkty_train);
	std::default_random_engine silnik (std::chrono::system_clock::now().time_since_epoch().count());
	std::shuffle(punkty_train.begin(), punkty_train.end(), silnik);

	zapisz(punkty_train, plik_punkty);
	
    auto odleglosci = wylicz_odleglosci(punkty_train);
	
	zapisz(odleglosci, plik_odleglosci);

}



int main ()
{
	const std::string roz_punkty {".punkty"};
	const std::string roz_odl {".odleglosci"};
	

	for (const auto n : {10, 20, 50, 100, 200, 500, 1000})
	{		
		std::cout << n << std::endl;
		auto nazwa = std::to_string(n);
		generuj_dane_dla_single_complete_link(nazwa + roz_punkty, nazwa + roz_odl, n);
	}
	return 0;
}


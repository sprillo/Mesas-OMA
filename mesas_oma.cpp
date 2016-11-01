#include<iostream>
#include<vector>
#include<map>
#include<assert.h>

using namespace std;

#define forn(i,n) for(int i = 0; i < (int)n; i++)

typedef long long int tint;

const int MAXC = 64;

class mesas_y_niveles{
	public:
	mesas_y_niveles();
	int solve();
	
	private:
	
	struct state{
		int pos;///desde esta columna para adelante quiero llenar
		int primera;///la primer columna tiene que ser de este nivel
		int n[3];///la cantidad de alumnos de cada nivel que me falta asignar
		state(int z, int a, int b, int c, int d) : pos(z), primera(a) {n[0] = b; n[1] = c; n[2] = d;}
		void show(){
			cout << pos << " ";
			cout << primera << " ";
			forn(i,3)
				cout << n[i] << " ";
			cout << endl;
		}
		state() {}
	};

	static bool comparator(state s1, state s2){
		if(s1.pos < s2.pos) return true;
		if(s1.pos > s2.pos) return false;
		if(s1.primera < s2.primera) return true;
		if(s1.primera > s2.primera) return false;
		forn(i,3){
			if(s1.n[i] < s2.n[i]) return true;
			if(s1.n[i] > s2.n[i]) return false;
		}
		return false;
	}
	
	int get(state s);
	void output(tint n1, tint n2, tint n3);
	void reconstruirSolucionDesde(state s, tint & n1, tint & n2, tint & n3);
	
	int cantColumnas;///cantidad total de columnas entre los 3 salones
	int cantSalones;///cantidad de salones
	int cantNivel[3];///cantidad de mesas que hay que acomodar por nivel
	int columnasSalon[MAXC];///cantidad de columnas que hay en cada salon
	int columnas[MAXC];///la cantidad de mesas por columnas. Las columnas de los salones se punen consecutivamente.
	bool nuevoSalon[MAXC];///esto me indica a partir de que columnas empieza un nuevo salon.
	int mesasQueSobran;///cuantas mesas sobran, lo uso para podar la dp
	map<state, int, bool(*)(state,state)> memo;
	map<state, state, bool(*)(state,state)> next;
};





/* Lee el input */
mesas_y_niveles::mesas_y_niveles(){
	cin >> cantColumnas >> cantSalones;
	if(cantColumnas >= MAXC){
		cerr << "Hay demasiadas columnas, solo me banco hasta 64" << endl;
		assert(false);
	}
	forn(i,3)
		cin >> cantNivel[i];
	forn(i,MAXC)
		nuevoSalon[i] = false;
	int acum = 0;
	nuevoSalon[0] = true;
	forn(i,cantSalones){
		int cols;
		cin >> cols;
		columnasSalon[i] = cols;
		acum += cols;
		nuevoSalon[acum] = true;
	}
	nuevoSalon[cantColumnas] = true;
	
	forn(i,cantColumnas)
		cin >> columnas[i];
	
	mesasQueSobran = 0;
	forn(i,3) mesasQueSobran -= cantNivel[i];
	forn(i,cantColumnas) mesasQueSobran += columnas[i];
	if(mesasQueSobran < 0){
		cerr << "Hay mas mesas que alumnos, revisar en input" << endl;
		assert(false);
	}
}

void mesas_y_niveles::output(tint n1, tint n2, tint n3){
	cout << "Encontre la solucion:" << endl;
	
	tint total = n1 | n2 | n3;
	assert(total == (1LL << cantColumnas) - 1);
	
	int tot1 = 0;
	int tot2 = 0;
	int tot3 = 0;
	
	int acum = 0;
	forn(i,cantSalones){
		forn(j,columnasSalon[i]){
			int k = acum + j;
			if(n1 >> k & 1){
				cout << "1 ";
				tot1 += columnas[k];
			}
			if(n2 >> k & 1){
				cout << "2 ";
				tot2 += columnas[k];
			}
			if(n3 >> k & 1){
				cout << "3 ";
				tot3 += columnas[k];
			}
		} cout << endl;
		acum += columnasSalon[i];
	}
	assert(tot1 >= cantNivel[0]);
	assert(tot2 >= cantNivel[1]);
	assert(tot3 >= cantNivel[2]);
}

void mesas_y_niveles::reconstruirSolucionDesde(state s, tint & n1, tint & n2, tint & n3){
	if(s.pos == cantColumnas)
		return;
	else{
		tint * nx = 0;
		if(s.primera == 0) nx = & n1;
		if(s.primera == 1) nx = & n2;
		if(s.primera == 2) nx = & n3;
		*nx += (1LL << s.pos);
		reconstruirSolucionDesde(next[s],n1,n2,n3);
	}
}

int mesas_y_niveles::get(state s){
	///si ya lo tengo
	if(memo.count(s)){
		return memo[s];
	}
	///si no lo tengo
	int & res = memo[s];
	res = -1;
	if(s.pos == cantColumnas){///si es caso base
		if(s.n[0] <= 0 && s.n[1] <= 0 && s.n[2] <= 0){///si es una caso base
			res = 0;
		}
		else{
			res = -1;
		}
	}
	else{
		///no llegue al final
		///tengo dos/tres formas de continuar la construccion.
		res = -1;///pongo el valor actual del estado en 'no hay solucion'.
		int newN[3];///cuantos alumnos quedan por nivel
		forn(i,3)
			newN[i] = (i == s.primera) ? s.n[i] - columnas[s.pos] : s.n[i];
		bool todaviaTieneSentido = true;
		forn(i,3)
			todaviaTieneSentido &= newN[i] >= -mesasQueSobran;
		if(todaviaTieneSentido){///solo en este caso hago la recursion
			forn(siguiente,3){
				if((siguiente != s.primera) || (nuevoSalon[s.pos + 1] || (s.pos + 1 == cantColumnas))){
					state t(s.pos + 1,siguiente,newN[0],newN[1],newN[2]);
					int resRec = get(t);
					if(resRec == -1)///no habia solucion.
						continue;
					else{
						///es solucion.
						///calculo el costo de la solucion actual.
						int cost = resRec;
						///tengo que sumar 1 si puse el nivel 3, pero no lo complete todavia.
						if(s.n[2] > 0 && s.n[2] < cantNivel[2]){
							///si lo puse pero no lo complete
							cost++;
						}
						else{
							;///no tengo que sumar nada.
						}
						///ya calcule el costo, ahora veo si mejoro la solucion
						if((res == -1) || (res > cost)){
							res = cost;
							next[s] = t;
						}
					}
				}
			}
		}
	}
	
	return res;
}

int mesas_y_niveles::solve(){
	bool(*cmp)(state,state) = mesas_y_niveles::comparator;
	map<state, int, bool(*)(state,state)> myMap(cmp);
	map<state, state, bool(*)(state,state)> myMap2(cmp);
	memo = myMap;
	next = myMap2;
	int res = -1;
	tint n1, n2, n3;
	forn(nivelDeLaPrimerColumna,3){
		state s(0,nivelDeLaPrimerColumna,cantNivel[0],cantNivel[1],cantNivel[2]);
		int resCandidato = get(s);
		if(resCandidato != -1 && ((res == -1) || (res > resCandidato))){
			res = resCandidato;
			n1 = n2 = n3 = 0;
			reconstruirSolucionDesde(s,n1,n2,n3);
		}
	}
	if(res != -1)
		output(n1,n2,n3);
	return res;
}




int main(){
	FILE * f = freopen("in.txt","r",stdin);

	mesas_y_niveles M;
	
	int res = M.solve();
	
	if(res == -1)
		cout << "No hay solucion!!!" << endl;
	else
		cout << "La minima distancia posible entre columnas del nivel 3 es " << res << "." << endl;
	
	fclose(f);
	
    return 0;
}

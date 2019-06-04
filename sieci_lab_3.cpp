#include <cstdint>
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>
#include <set>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <math.h>
using namespace std;
struct edge {
	int v1;
	int v2;
	int w;
};
struct vertex {
	int x;
	int y;
};
bool ByWeight(const edge &a, const edge &b)
{
	return a.w < b.w;
}
bool find_vec(vector <int> vec, int x) {
	vector<int>::iterator it;
	it = find(vec.begin(), vec.end(), x);
	if (it != vec.end())
		return true;
	else
		return false;
}
void copy_vec(vector <vector <int>> &L_K,int x,int y) {
	int xx, yy;
	// << x << "  " << y << endl;
	for (int i = 0; i < L_K.size(); i++) {
		if (find_vec(L_K[i], x)) xx = i;
	}
	for (int i = 0; i < L_K.size(); i++) {
		if (find_vec(L_K[i], y)) yy = i;
	}
	for (int i = 0; i < L_K[yy].size(); i++) {
		L_K[xx].push_back(L_K[yy][i]);
	}
	for (int i = 0; i < L_K[yy].size(); i++) {
		L_K[yy].erase(L_K[yy].begin());
	}
}
bool is_full(vector <vector <int>> L_K , int vertices) {
	bool flag = false;
	for (int i = 0; i < L_K.size(); i++) {
		if (L_K[i].size() == vertices) flag= true;
	}
	return flag;
}

int Kruskal(vector <edge> K, vector <edge> &L,const int vertices) {
	vector <vector <int>> L_K;
	vector <int> tmp;
	tmp.push_back(0);
	bool flag = false;
	for (int i = 0; i < vertices; i++) {
		tmp[0] = i+1;
		L_K.push_back(tmp);
	}
	sort(K.begin(), K.end(), ByWeight);
	while (!is_full(L_K,vertices)) {
		for (int i = 0; i < L_K.size(); i++) {
			if (find_vec(L_K[i], K[0].v1 ) && find_vec(L_K[i], K[0].v2)) flag = true;
		}
		if (!flag) {
			copy_vec(L_K, K[0].v1, K[0].v2);
			L.push_back(K[0]);
		}
		
		K.erase(K.begin());
		flag = false;
	}
	int koszt = 0;
	for (int i = 0; i < L.size(); i++) koszt += L[i].w;

	return koszt;
}

bool is_added(vector <int> V, int x)
{
	for (int i = 0; i < V.size(); i++) {
		if (V[i] == x) {
			return  true;
		}
	}
	return false;
}
void set_if_not_exists(vector <edge> &queue, edge x) {
	bool exists = false;
	for(int i=0;i<queue.size();i++) 
		if ((queue[i].v1 == x.v1) &&(queue[i].v2 == x.v2)&&(queue[i].w == x.w)) {
			exists = true;
		}
	
	if(exists==false) queue.push_back(x);
}
void Set_queue(vector <edge> K, vector <edge> &queue,vector <int> V) {
	for (int i = 0; i < V.size(); i++) {
		for (int j = 0; j < K.size(); j++) {
			if (K[j].v1 == V[i]&&!is_added(V,K[j].v2) || K[j].v2 == V[i] && !is_added(V, K[j].v1)) {
				set_if_not_exists(queue, K[j]);
			}
		}
	}
	sort(queue.begin(), queue.end(), ByWeight);
}
void Remove_edge(vector <edge> &K, edge x) {
	for (int i = 0; i < K.size(); i++) {
		if (K[i].v1 == x.v1 &&K[i].v2 == x.v2&&K[i].w== x.w) {
			K.erase(K.begin() + i);
			break;
		}
	}
}

int Prima(vector <edge> K, vector <edge> &L, const int vertices)
{
	vector <int> V;
	vector <edge> queue;
	int koszt = 0;
	srand(time(NULL));
	V.push_back(1+rand()%vertices);
	while (V.size() < vertices) {
		Set_queue(K, queue, V);
		if (is_added(V,queue[0].v1)) {
			V.push_back(queue[0].v2);
		}
		else V.push_back( queue[0].v1);
		L.push_back(queue[0]);
		Remove_edge(K,queue[0]);
		queue.erase(queue.begin(),queue.end());
	}
	for (int i = 0; i < L.size(); i++) koszt += L[i].w;
	return koszt;
}
void Draw(vector <edge> &L, int vertices,char * name,int cost , clock_t time_of_exec) {
	al_init();
	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();


	ALLEGRO_FONT *font = al_create_builtin_font();
	ALLEGRO_KEYBOARD_STATE klawiatura;
	ALLEGRO_DISPLAY *okno = al_create_display(520, 520);
	al_set_window_title(okno, name);
	ALLEGRO_EVENT_QUEUE *kolejka = al_create_event_queue(); // event_queue
	al_register_event_source(kolejka, al_get_display_event_source(okno));
	
	vector <vertex> t;
	vertex xd;
	int licz = 0;
	al_clear_to_color(al_map_rgb(0, 0, 0)); //t³o
	for (int i = 1; i < sqrt(vertices) + 1; i++) {
		for (int j = 1; j <sqrt(vertices) + 1; j++) {
			if (licz < vertices) {
				licz++;
				xd.x = j * 500 / (sqrt(vertices) + 1);
				xd.y = i * 500 / (sqrt(vertices) + 1);
				t.push_back(xd);

			}
		}
	}
	srand(time(NULL));
	ALLEGRO_COLOR x;
	for (int i = 0; i < L.size(); i++) {
		x = al_map_rgb(150 + rand() % 105, rand() % 255,rand() %255);
		al_draw_line(t[L[i].v1 - 1].x + i/2, t[L[i].v1 - 1].y + i/2, t[L[i].v2 - 1].x + i/2, t[L[i].v2 - 1].y + i/2, x, L[i].w / 2);
		if (t[L[i].v1 - 1].x>t[L[i].v2 - 1].x)
			al_draw_textf(font, x, (t[L[i].v1 - 1].x + t[L[i].v2 - 1].x - 20) / 2, (t[L[i].v1 - 1].y + t[L[i].v2 - 1].y - 30) / 2, 0, "%d", L[i].w);

		else al_draw_textf(font, x, (t[L[i].v1 - 1].x + t[L[i].v2 - 1].x + 20) / 2, (t[L[i].v1 - 1].y + t[L[i].v2 - 1].y - 30) / 2, 0, "%d", L[i].w);
	}
	for (int j = 0; j <vertices; j++) {

		al_draw_filled_circle(t[j].x, t[j].y, 10, al_map_rgb(255, 1, 0));
		al_draw_textf(font, al_map_rgb(0, 0, 0), t[j].x, t[j].y, 5, "%d", j + 1);


	}
	if (cost != 0) {
		al_draw_textf(font, al_map_rgb(255, 255, 255), 40, 20, ALLEGRO_ALIGN_LEFT, "Koszt: %d   czas: %f s", cost, ((float)time_of_exec) / (float)CLOCKS_PER_SEC);
	}
	while (1)
	{
	ALLEGRO_EVENT zdarzenie; // event
	al_get_next_event(kolejka, &zdarzenie);
	if (zdarzenie.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { break; }
	al_get_keyboard_state(&klawiatura);
	al_flip_display();
	}
	al_destroy_display(okno);
}
int main() {
	
	
	const int vertices =6;
	vector <edge> input{ {1,2,1}, {1,3,2}, {1,4,2}, {1,5,6}, {2,4,4},{2,5,5},{3,4,3},{4,5,3},{ 2,3,1 },{ 4,6,1 },{ 6,2,1 },{ 6,5,2 },{ 3,6,1 }}; ////(krawedz,krawêdŸ,waga)
	vector <edge> output_1, output_2;

	Draw(input,vertices,(char*)"drzewo poczatkowe",0,NULL);
	
	clock_t t = clock();
	int cost= Kruskal(input, output_1,vertices);
	t = clock() - t;
	Draw(output_1, vertices,(char*)"Kruskal",cost,t);

	t = clock();
	cost = Prima(input, output_2, vertices);
	t = clock() - t;
	Draw(output_2, vertices, (char*)"Prim", cost,t);

	system("pause");
	return 0;
}
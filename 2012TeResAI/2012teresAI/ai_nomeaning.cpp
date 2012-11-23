/**********************************************************
	AIのサンプル
**********************************************************/

#include "Data.h"
#include <cassert>
#include <queue>
#include "windows.h"
#include <string>
#include <sstream>
#include <cstring>

#define NOMEANING_DEBUG
#undef NOMEANING_DEBUG

namespace {
	const int MERGIN = 100;
	int tx, ty;
	int dist[MERGIN * 2][MERGIN * 2];
	int MAP[MERGIN * 2][MERGIN * 2];
	enum{EMPTY,WALL,AI,ENEMY};
	// しょきか
	void init(){
		tx = MERGIN;
		ty = MERGIN;
		for(int i = 0; i < MERGIN * 2; i++){
			for(int j = 0; j < MERGIN * 2; j++){
				MAP[i][j] = WALL;
			}
		}
	}
	// きんぼー
	int dx[4] = {1, 0, -1, 0},
		dy[4] = {0, 1, 0, -1};
	Action d[4] = {E, S, W, N};
	string d_s[4] = {"E", "S", "W", "N"};
}

/**********************************************************
	初期化時に呼ばれる関数
**********************************************************/
void AINomeaningInit(AI_T &myAi)
{
	myAi.Graph = LoadGraph("AI_image\\blue.png");  //画像の設定
	strcpy_s(myAi.name, "nomeaningAI");  //自分のAIの名前設定
	init();
}


void output_number(int num){
	stringstream ss;
	ss<<num<<"\n";
	OutputDebugString(ss.str().c_str());
}

/**********************************************************
	AIの行動を返す関数
**********************************************************/
Action AINomeaning(int view[2*VISIBLE+1][2*VISIBLE+1])
{
	const int N = 2 * VISIBLE + 1; // 配列の大きさ
	int px = VISIBLE + tx, py = VISIBLE + ty; // プレイヤーのざひょー
	Action ret = STOP; // もどりち
	int ex = -1, ey = -1; // 敵のざひょー
	
	// マップを確定させる
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			if(view[x][y] != WALL){
				MAP[x + tx][y + ty] = EMPTY;
			}
			if(view[x][y] == ENEMY){
				ex = x + tx; ey = y + ty;
				output_number(12345);
			}
			// output_number(view[x][y]);

			if((MAP[x + tx][y + ty] == EMPTY && view[x][y] == WALL)){
				output_number(987654321);
				init();
			}
		}
	}
	// でばっぐ用の表示
#ifdef NOMEANING_DEBUG
	OutputDebugString("-------\n");
	for(int y = 0; y < N; y++){
		for(int x = 0; x < N; x++){
			string to;
			to.push_back(view[x][y] + '0');
			OutputDebugStringA(to.c_str());
		}
		OutputDebugString("\n");
	}
	Sleep(1000);
#endif
	if(ex != -1){
		int tmp = 0;
		const int INF = -1;
		const int T = 10;
		// BFS
		typedef pair<int,int> P;
		memset(dist, -1, sizeof(dist));
		queue<P> que;
		output_number(12345);
		dist[ex][ey] = 0;
		que.push(P(ex,ey));
		while(!que.empty()){
			P now = que.front(); que.pop();
			for(int k = 0; k < 4; k++){
				int nx = now.first + dx[k], ny = now.second + dy[k];
				assert(nx >= 0 && nx < MERGIN * 2 && ny >= 0 && ny < MERGIN * 2);
				if(dist[nx][ny] == INF && MAP[nx][ny] != WALL){
					dist[nx][ny] = dist[now.first][now.second] + 1;
					output_number(dist[nx][ny]);
					que.push(P(nx,ny));
				}
			}
			tmp++;
		}
		#ifdef NOMEANING_DEBUG
	OutputDebugString("-dist-\n");
	for(int y = 0; y < N; y++){
		for(int x = 0; x < N; x++){
			string to;
			to.push_back(dist[x+tx][y+ty] + '0');
			OutputDebugStringA(to.c_str());
		}
		OutputDebugString("\n");
	}
	Sleep(1000);
#endif
		// output_number(tmp);
		int min_dist = dist[px][py];
		for(int k = 0; k < 4; k++){
			int xx = px + dx[k],
				yy = py + dy[k];
			if(dist[xx][yy] >= min_dist){
				min_dist = dist[xx][yy];
				ret = d[k];
				OutputDebugString(d_s[k].c_str());
				output_number(min_dist);
			}
		}
	}else{
		ret = d[rand()%4];
	}

	// 移動場所に応じて情報を更新する
	if(ret == E){
		tx += 1;
		ty += 0;
	}else if(ret == W){
		tx -= 1;
		ty += 0;
	}else if(ret == N){
		tx += 0;
		ty -= 1;
	}else if(ret == S){
		tx += 0;
		ty += 1;
	}
	return ret;
	/**
		Action: E S W N STOPかな
	*/
}

#undef NOMEANING_DEBUG
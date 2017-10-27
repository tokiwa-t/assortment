#include<stdio.h>
#include<stdlib.h> // rand関数を使用するために必要
#include<time.h> // time関数を使用するために必要

#define MS_SIZE 8 // ゲームボードのサイズを定義(変更不可)
#define MINE_NUMBER 10 // 地雷の個数を指定

int main(void){
    int mode; // モードを保持(モード: セルを開く, フラグを立てる/除去する)
    int number_of_mines = 10; // 地雷数のデフォルト値は10
    int mine_map[MS_SIZE][MS_SIZE]; // 地雷セルと数字セルを記録
    int game_board[MS_SIZE][MS_SIZE]; // ゲームの進行を記録するためのゲームボード
    
    // 必要な変数は以下に適宜追加する
    int i, j;
    int count; // 課題2: 地雷カウンタ, 課題3,4: 開いていないセルのカウンタ
    int judge; // ゲームが終了(クリア = 1 または ゲームオーバー = -1)したら非0にする
    int repeat; // ゲームを続けるかどうか

    srand((unsigned)time(NULL)); // time関数で現在時刻を取得し, その値により乱数を初期化する.alignas
    
    do{

        /* 課題1 地雷をランダムにセット */
        count = 0;
        for(i=0;i<MS_SIZE;i++){ // 1行ごとに走査
            for(j=0;j<MS_SIZE;j++){ // 1列ごとに走査
                if(count <= MINE_NUMBER)
                    mine_map[i][j] = rand() % 2 * (-1); // 0(何も無し)か-1(地雷)をセットする
                else
                    mine_map[i][j] = 0;

                    /*------------------------------*/
                    /* rand()%2 は0か1をランダムに返す */
                    /* (2の剰余なので, 0か1が返る)     */
                    /*------------------------------*/

                if(mine_map[i][j]==-1){
                    count++;
                }

                // printf("%2d ", mine_map[i][j]);

                game_board[i][j] = 0; // ゲームボードの初期化
            }
            // printf("\n");
        }

        // printf("\n");

        /* 課題2 各セルの8近傍の地雷をカウント */
        for(i=0;i<MS_SIZE;i++){ // 1行ごとに走査
            for(j=0;j<MS_SIZE;j++){ // 1列ごとに走査
            
                if(mine_map[i][j] != -1){ // 自セルに地雷がない場合, 周囲を確認

                    count = 0; // 地雷カウンタの初期化
                    // 自セルから見て, 上から時計回りに地雷の有無を確認する

                    if(i!=0){ // 先頭行を除く
                        if(mine_map[i-1][j] == -1){
                            count++; // 上が地雷ならカウント
                        }
                    }

                    if(i!=0&&j!=7){ // 先頭行または末尾列を除く
                        if(mine_map[i-1][j+1] == -1){
                            count++; // 右上が地雷ならカウント
                        }
                    }

                    if(j!=7){ // 末尾列を除く
                        if(mine_map[i][j+1] == -1){
                            count++; // 右が地雷ならカウント
                        }
                    }

                    if(i!=7&&j!=7){ // 末尾行または末尾列を除く
                        if(mine_map[i+1][j+1] == -1){
                            count++; // 右下が地雷ならカウント
                        }
                    }

                    if(i!=7){ // 末尾行を除く
                        if(mine_map[i+1][j] == -1){
                            count++; // 下が地雷ならカウント
                        }
                    }

                    if(i!=7&&j!=0){ // 末尾行または先頭列を除く
                        if(mine_map[i+1][j-1] == -1){
                            count++; // 左下が地雷ならカウント
                        }
                    }

                    if(j!=0){ // 先頭列を除く
                        if(mine_map[i][j-1] == -1){
                            count++; // 左が地雷ならカウント
                        }
                    }

                    if(i!=0&&j!=0){ // 先頭行または先頭列を除く
                        if(mine_map[i-1][j-1] == -1){
                            count++; // 左上が地雷ならカウント
                        }
                    }

                    mine_map[i][j] = count; // カウントした地雷の数を自セルに格納
                    
                }
                // printf("%2d ", mine_map[i][j]);
            }
            // printf("\n");
        }

        /* ここから表示部 */
        for(i=0;i<MS_SIZE;i++){
            printf("%d| ", i);
            for(j=0;j<MS_SIZE;j++){
                if(game_board[i][j]==0){
                    printf(" x ");
                }else if(game_board[i][j]==1){
                    printf("%2d ", mine_map[i][j]);
                }else{
                    printf(" F ");
                }
            }
            printf("\n");
        }
        printf("   ");
        for(j=0;j<MS_SIZE;j++){
            printf("---");
        }
        printf("\n");
        printf("   ");
        for(j=0;j<MS_SIZE;j++){
            printf("%2d ", j);
        }
        printf("\n");
        /* ここまで表示部 */

        judge = 0;
        while(judge == 0){ // 判定変数が非0になるまで繰り返す
            count = 0; // まだ開いていないセルがあればカウント

            printf("モードを選択してください: セルを開く(1), フラグを設置/除去する(2): ");
            scanf("%d", &mode);
            while(mode!=1 && mode!=2){
                printf("モードを選択してください: セルを開く(1), フラグを設置/除去する(2): ");
                scanf("%d", &mode);
            }

            /* ここでユーザがセルを指定 */

            if(mode==1){
                /*
                    課題3 セルを開ける. 地雷があればゲームオーバ.
                    地雷が無ければ選択したセルと周囲8近傍のセルを開ける
                */
                
                printf("セルを空白区切りで指定してください: ");
                scanf("%d %d", &i, &j);

                if(mine_map[i][j] != -1){ // 自セルに地雷がない場合, 周囲を確認
                    
                    count = 0; // 地雷カウンタの初期化
                    // 自セルから見て, 上から時計回りに地雷の有無を確認する
                    
                    if(i!=0){ // 先頭行を除く
                        if(mine_map[i-1][j] != -1){
                            game_board[i-1][j] = 1; // 上が地雷でなければ開く
                        }
                    }
                    
                    if(i!=0&&j!=7){ // 先頭行または末尾列を除く
                        if(mine_map[i-1][j+1] != -1){
                            game_board[i-1][j+1] = 1; // 右上が地雷でなければ開く
                        }
                    }
                    
                    if(j!=7){ // 末尾列を除く
                        if(mine_map[i][j+1] != -1){
                            game_board[i][j+1] = 1; // 右が地雷でなければ開く
                        }
                    }
                    
                    if(i!=7&&j!=7){ // 末尾行または末尾列を除く
                        if(mine_map[i+1][j+1] != -1){
                            game_board[i+1][j+1] = 1; // 右下が地雷でなければ開く
                        }
                    }
                    
                    if(i!=7){ // 末尾行を除く
                        if(mine_map[i+1][j] != -1){
                            game_board[i+1][j] = 1; // 下が地雷でなければ開く
                        }
                    }
                    
                    if(i!=7&&j!=0){ // 末尾行または先頭列を除く
                        if(mine_map[i+1][j-1] != -1){
                            game_board[i+1][j-1] = 1; // 左下が地雷でなければ開く
                        }
                    }
                    
                    if(j!=0){ // 先頭列を除く
                        if(mine_map[i][j-1] != -1){
                            game_board[i][j-1] = 1; // 左が地雷でなければ開く
                        }
                    }
                    
                    if(i!=0&&j!=0){ // 先頭行または先頭列を除く
                        if(mine_map[i-1][j-1] != -1){
                            game_board[i-1][j-1] = 1; // 左上が地雷でなければ開く
                        }
                    }

                    game_board[i][j] = 1;
                                    
                }else{ // 指定したセルが地雷だった場合
                    judge = -1;  // ゲームオーバーにする
                }

            }else{
                /*
                    課題4 選択されたセルにフラグを設置する
                */

                printf("セルを空白区切りで指定してください: ");
                scanf("%d %d", &i, &j);

                if(game_board[i][j] != 2) // 指定されたセルに先にフラグが設定されていない場合
                    game_board[i][j] = 2; // 指定されたセルにフラグを設定
                else // そうでない場合
                    game_board[i][j] = 0; // フラグを解除
            }

            /* ここから表示部 */
            for(i=0;i<MS_SIZE;i++){
                printf("%d| ", i);
                for(j=0;j<MS_SIZE;j++){
                    if(game_board[i][j]==0){
                        printf(" x ");
                    }else if(game_board[i][j]==1){
                        printf("%2d ", mine_map[i][j]);
                    }else{
                        printf(" F ");
                    }
                }
                printf("\n");
            }
            printf("   ");
            for(j=0;j<MS_SIZE;j++){
                printf("---");
            }
            printf("\n");
            printf("   ");
            for(j=0;j<MS_SIZE;j++){
                printf("%2d ", j);
            }
            printf("\n");
            /* ここまで表示部 */

            for(i=0;i<MS_SIZE;i++){
                for(j=0;j<MS_SIZE;j++){
                    if(game_board[i][j] == 0 && mine_map[i][j]!=-1)
                        count++;
                }
            }
            if(count==0){
                printf("Congratulation!\n");
                judge = 1;
            }

            if(judge == -1)
                printf("ゲームオーバー\n");
        }

        printf("ゲームを続けますか？: はい(0), いいえ(1): ");
        scanf("%d", &repeat);

    }while(repeat == 0);

    return 0;
}
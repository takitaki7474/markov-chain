enum {
  NPREF = 2, /*プレフィクスの関数*/
  NHASH = 4093,/*状態ハッシュテーブル*/
  MAXGEN = 10000/*生成される単語数の上限*/
};

typedef struct State State;
typedef struct Suffix Suffix;

struct State {
  char *pref[NPREF]; /*プレフィクスの単語*/
  Suffix *suf;/*サフィックスリスト*/
  State *next;/*ハッシュテーブルの中の次の要素*/
};

struct Suffix {
  char *word;/*suffix*/
  Suffix *next;/*サフィックスッリストの中の次の要素*/
};

State *statetab[NHASH]; /*状態のハッシュテーブル*/

/*hash: NPREF個の文字列からなる配列のハッシュ値を計算*/
unsigned int hash(char *s[NPREF])
{
  unsigned int h;
  unsigned char *p;
  int i;

  h = 0;
  for(i = 0; i < NPREF; i++){
    for(p = (unsigned char *) s[i]; *p != '/0'; p++){
      h = MULTIPLIER * h + *p;
    }
  }
  return h % NHASH;
}

/*lookup: プレフィクスの検索、指定されればそれを生成*/
/*見つかるか生成ひたらポインタを、そうでなければnullを返す*/
/*生成作業はstrdupしないので、あとで文字列が変化しなければならない*/
State* lookup(char *prefix[NPREF], int create)
{
  int i, h;
  State *sp;

  h = hash(prefix);
  for(sp = statetab[h]; sp != NULL; sp = sp->next){
    for(i = 0; i < NPREF; i++){
      if(strcmp(prefix[i],sp->pref[i]) != 0){
        break;
      }
    }
    if(i == NPREF){
      return sp;
    }
  }
  if(create){
    sp = (State *) emalloc(sizeof(State));
    for(i = 0; i < NPREF; i++){
      sp->pref[i] = prefix[i];
    }
    sp->suf=NULL;
    sp->next=statetab[h];
    statetab[h] = sp;
  }
  return sp;
}

/*build: 入力を読み、プレフィクステーブルを作成*/
void build(char *prefix[NPREF], FILE *f)
{
  char buf[100], fmt[10];

  /*書式文字列を作成*/
  sprintf(fmt, )

}

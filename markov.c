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
  sprintf(fmt, "%%%ds", sizeof(buf)-1);
  while(fscanf(f, fmt, buf) != EOF){
    add(prefix, estrdup(buf));
  }
}

void add(char *prefix[NPREF], char *suffix)
{
  State *sp;

  sp = lookup(prefix, 1);
  addsuffix(sp, suffix);
  memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
  prefix[NPREF-1] = suffix;
}

void addsuffix(State *sp, char *suffix)
{
  Suffix *suf;

  suf = (Suffix *) emalloc(sizeof(Suffix));
  suf->word = suffix;
  suf->next = sp->suf;
  sp->suf = suf;
}

void generate(int nwords)
{
  State *sp;
  Suffix *suf;
  char *prefix[NPREF], *w;
  int i, nmatch;

  for(i = 0; i < NPREF; i++){/*初期プレフィクスをリセット*/
    prefix[i] = NONWORD;
  }
  for(i = 0; i < nwords; i++){
    sp = lookup(prefix, 0);
    nmatch = 0;
    for(suf = sp->suf; suf != NULL; suf = suf->next){
      if(rand() % ++nmatch == 0){/*確率1/nmatch*/
        w = suf->word
      }
    }
    if(strcmp(w, NONWORD) == 0){
      break;
    }
    printf("%s\n",w);
    memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
    prefix[NPREF-1] = w;
  }
}

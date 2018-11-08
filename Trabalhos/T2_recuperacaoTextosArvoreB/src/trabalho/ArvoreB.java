package trabalho;
import java.io.*;
import java.util.*;

public class ArvoreB{
	
	private Pagina raiz;
	private int min, max;
	private ArrayList<String> stopwords;
	
	public ArvoreB(int t){
		this.raiz = null;
		this.min = t-1;
		this.max = 2*t-1;
		this.stopwords = new ArrayList<String>();
	}
	public ArrayList<String> getStopwords(){
		return stopwords;
	}
	private static class Pagina{
		int n;
		MeuItem r[];
		Pagina p[]; 
		
		public Pagina(int max){
			this.n = 0;
			this.r = new MeuItem[max];
			this.p = new Pagina[max+1];
		}
	}
	
	// método para criação da lista de stopwords
	public void criarListaStopWords(String caminho){
		try{
			FileReader arquivo = new FileReader("stopwords.txt");
			BufferedReader lerArq = new BufferedReader(arquivo);
			String linha = lerArq.readLine(); // le primeira linha. EOF: linha == null.

			do{
				//System.out.println(linha);  //imprime stopwords pra conferir
				this.stopwords.add(linha);
				linha = lerArq.readLine(); // le da segunda ate a ultima linha
			}while (linha != null);
			
			arquivo.close();
		}
		catch(IOException e){
			System.err.printf("Erro na abertura do arquivo: %s.\n", e.getMessage());
		}
	}
	
	//método para verificar se a raiz é null
	public boolean raizEhNull(){
		if(this.raiz == null) return true;
		else return false;
	}
	
	//métodos para inserção de um item na Árvore B
	private void insereNaPagina(Pagina pag, MeuItem item, Pagina apDir){
		int k = pag.n - 1;
		while ((k >= 0) && (item.compara (pag.r[k]) < 0)) {
			pag.r[k+1] = pag.r[k];
			pag.p[k+2] = pag.p[k+1];
			k--; 
		}
		pag.r[k+1] = item;
		pag.p[k+2] = apDir;
		pag.n++;     
	}
	
	private Pagina insere(MeuItem novo, Pagina raiz, MeuItem[] itemRetorno, boolean[] cresceu, int[] repetido){
		Pagina pagRetorno = null;
		if(raiz == null){
			cresceu[0] = true;
			itemRetorno[0] = novo;
		} 
		else{
			int i = 0;
			while((i < raiz.n-1) && (novo.compara (raiz.r[i]) > 0)) i++;      
			if(novo.compara(raiz.r[i]) == 0){
				System.out.println("Erro: Registro ja existente");
				repetido[0] = 1;
				cresceu[0] = false;
			} 
			else{
				if(novo.compara(raiz.r[i]) > 0) i++; //se o codigo do novo é maior, anda pra direita
				pagRetorno = insere(novo, raiz.p[i], itemRetorno, cresceu, repetido);
				if(cresceu[0])          
					if(raiz.n < this.max){ // se ainda tem espaço na pagina
						this.insereNaPagina(raiz, itemRetorno[0], pagRetorno);
						cresceu[0] = false; pagRetorno = raiz;
					} 
					else{ // pagina atingiu tamanho maximo, tem de ser dividida
						Pagina pagTemp = new Pagina(this.max);
						pagTemp.p[0] = null;
						if(i <= this.min){
							this.insereNaPagina(pagTemp, raiz.r[this.max-1], raiz.p[this.max]);
							raiz.n--;
							this.insereNaPagina(raiz, itemRetorno[0], pagRetorno);
						}
						else this.insereNaPagina(pagTemp, itemRetorno[0], pagRetorno);
						for(int j = this.min+1; j < this.max; j++) {
							this.insereNaPagina(pagTemp, raiz.r[j], raiz.p[j+1]);
							raiz.p[j+1] = null;
						}
						raiz.n = this.min;
						pagTemp.p[0] = raiz.p[this.min+1]; 
						itemRetorno[0] = raiz.r[this.min];
						pagRetorno = pagTemp;
					}
			}        
		}
		return(cresceu[0] ? pagRetorno : raiz);
	}
	
	public int[] insere(MeuItem novo, int[] repetido){
		repetido[0] = 0;
		MeuItem itemRetorno[] = new MeuItem[1];
		boolean cresceu[] = new boolean[1];
		Pagina pagRetorno = this.insere(novo, this.raiz, itemRetorno, cresceu, repetido);
		if(cresceu[0]){
			Pagina pagTemp = new Pagina(this.max);
			pagTemp.r[0] = itemRetorno[0];
			pagTemp.p[0] = this.raiz;
			pagTemp.p[1] = pagRetorno;
			this.raiz = pagTemp;
			this.raiz.n++;
		}
		else this.raiz = pagRetorno;
		return repetido;
	}
	
	//métodos para remoção de um item da Árvore B
	private boolean reconstitui(Pagina apPag, Pagina apPai, int posPai){
		boolean diminuiu = true;
		if(posPai < apPai.n){ // se está na página à direita de aPag
			Pagina aux = apPai.p[posPai+1];
			int dispAux = (aux.n - this.min + 1)/2;
			apPag.r[apPag.n++] = apPai.r[posPai];
			apPag.p[apPag.n] = aux.p[0];
			aux.p[0] = null; // transfere a posse da memória
			if(dispAux > 0){ // existe folga: transfere de aux para apPag
				for(int j = 0; j < dispAux - 1; j++){
					this.insereNaPagina(apPag, aux.r[j], aux.p[j+1]);
					aux.p[j+1] = null; // transfere a posse da memória
				}
				apPai.r[posPai] = aux.r[dispAux - 1];
				aux.n = aux.n - dispAux;
				for (int j = 0;  j < aux.n; j++) aux.r[j] = aux.r[j+dispAux];
				for (int j = 0; j <= aux.n; j++) aux.p[j] = aux.p[j+dispAux];
				aux.p[aux.n+dispAux] = null; // transfere a posse da memória
				diminuiu = false;
			}
			else{ // intercala aux em apPag e libera aux
				for(int j = 0; j < this.min; j++){
					this.insereNaPagina(apPag, aux.r[j], aux.p[j+1]);
					aux.p[j+1] = null; // transfere a posse da memória
				}
				aux = apPai.p[posPai+1] = null; //
				for(int j = posPai; j < apPai.n-1; j++){
					apPai.r[j] = apPai.r[j+1];
					apPai.p[j+1] = apPai.p[j+2]; 
				}
				apPai.p[apPai.n--] = null; // transfere a posse da memória
				diminuiu = apPai.n < this.min;
			}      
		}
		else{ // se está na página à esquerda de apPag
			Pagina aux = apPai.p[posPai-1];      
			int dispAux = (aux.n - this.min + 1)/2;
			for(int j = apPag.n-1; j >= 0; j--) apPag.r[j+1] = apPag.r[j];
			apPag.r[0] = apPai.r[posPai-1];
			for (int j = apPag.n; j >= 0; j--) apPag.p[j+1] = apPag.p[j];
			apPag.n++;
			if(dispAux > 0){ // existe folga: transfere de aux para apPag
				for(int j = 0; j < dispAux - 1; j++){
					this.insereNaPagina (apPag, aux.r[aux.n-j-1], aux.p[aux.n-j]);
					aux.p[aux.n-j] = null; // transfere a posse da memória
				}
				apPag.p[0] = aux.p[aux.n - dispAux + 1];
				aux.p[aux.n - dispAux + 1] = null; // transfere a posse da memória
				apPai.r[posPai-1] = aux.r[aux.n - dispAux];
				aux.n = aux.n - dispAux; diminuiu = false;
			}
			else{ // intercala aux em apPag e libera aux
				for(int j = 0; j < this.min; j++){
					this.insereNaPagina (aux, apPag.r[j], apPag.p[j+1]);
					apPag.p[j+1] = null; // transfere a posse da memória
				}
				apPag = null; // libera apPa
				apPai.p[apPai.n--] = null; // transfere a posse da memória
				diminuiu = apPai.n < this.min;
			}      
		}
		return diminuiu;
	}
	
	private boolean antecessor(Pagina ap, int ind, Pagina apPai){
		boolean diminuiu = true;
		if(apPai.p[apPai.n] != null){
			diminuiu = antecessor (ap, ind, apPai.p[apPai.n]);
			if(diminuiu) diminuiu = reconstitui(apPai.p[apPai.n],apPai,apPai.n);
		}
		else{
			ap.r[ind] = apPai.r[--apPai.n];
			diminuiu = apPai.n < this.min;
		}
		return diminuiu;
	}
	
	private Pagina remove(MeuItem itemRemover, Pagina ap, boolean[] diminuiu){
		if(ap == null){
			System.out.println("Erro: Registro nao encontrado");
			diminuiu[0] = false;
		}
		else{
			int ind = 0;
			while((ind < ap.n-1) && (itemRemover.compara (ap.r[ind]) > 0)) ind++;
			if(itemRemover.compara (ap.r[ind]) == 0){ // achou
				if(ap.p[ind] == null){ // se é página folha
					ap.n--;
					diminuiu[0] = ap.n < this.min;
					for(int j = ind; j < ap.n; j++){
						ap.r[j] = ap.r[j+1];
						ap.p[j] = ap.p[j+1];
					}
					ap.p[ap.n] = ap.p[ap.n+1]; 
					ap.p[ap.n+1] = null; // transfere a posse da memória
				}
				else{ // se a página não é folha, troca com antecessor
					diminuiu[0] = antecessor (ap, ind, ap.p[ind]);
					if (diminuiu[0]) diminuiu[0] = reconstitui (ap.p[ind], ap, ind);          
				}
			}
			else{ // não existe o item na árvore 
				if(itemRemover.compara (ap.r[ind]) > 0) ind++;
				ap.p[ind] = remove (itemRemover, ap.p[ind], diminuiu);
				if(diminuiu[0]) diminuiu[0] = reconstitui (ap.p[ind], ap, ind);
			}
		}
		return ap;
	}
	
	public void remove(MeuItem itemRemover){
		boolean diminuiu[] = new boolean[1];
		this.raiz = this.remove(itemRemover, this.raiz, diminuiu);
		if(diminuiu[0] && (this.raiz.n == 0)){ // árvore diminui em altura
			this.raiz = this.raiz.p[0];
		}
	}
	
	//métodos para query
	private void query(Pagina raiz, int nivel, String palavra, int[] nEncontrado){
		if(raiz != null){
			HashMap<String, Integer> vf = new HashMap<String, Integer>();
			for(int i=0; i<raiz.n; i++){
				vf = raiz.r[i].getVetorFreq();
				if(vf.containsKey(palavra) && vf.get(palavra) >= 3){
					nEncontrado[0] = 0;
					System.out.println("No. de ocorrências da palavra no texto " + raiz.r[i].getCodTexto() + ": " + vf.get(palavra));
					try{
						java.awt.Desktop.getDesktop().open(new File(raiz.r[i].getCaminhoTexto()));
					}catch (IOException e){
						e.printStackTrace();
					}
				}
			}
			for(int i = 0; i <= raiz.n; i++){
				query(raiz.p[i], nivel + 1, palavra, nEncontrado);      
			}
		}
	}
	
	public void query(String palavra){
		System.out.println("Buscando por textos nos quais palavra \"" + palavra + "\" aparece um número significativo de vezes...");
		int[] nEncontrado = new int[1]; 
		nEncontrado[0] = 1;
		this.query(this.raiz, 0, palavra, nEncontrado);
		if(nEncontrado[0]==1) System.out.println("Palavra não encontrada em quantidade significativa nos textos da árvore");
	}
	
	
	//métodos para busca na Árvore B
	private MeuItem busca(MeuItem item, Pagina pag){
		if(pag == null) return null; // item não encontrado na árvore
		else{
			int i = 0;
			while((i < pag.n-1) && (item.compara (pag.r[i]) > 0)) i++;
			if(item.compara (pag.r[i]) == 0) return pag.r[i];
			else if(item.compara(pag.r[i]) < 0) return busca(item, pag.p[i]);
			else return busca(item, pag.p[i+1]);
		}
	}

	public MeuItem busca(MeuItem item){
		return this.busca(item, this.raiz);
	}
	
	//métodos para impressão
	private void imprime(Pagina p, int nivel){
		if(p != null){
			System.out.print ("	Nivel " + nivel + ":");
			for(int i=0; i<p.n; i++){
				System.out.print(" "+p.r[i].toString());
				if(i == p.n -1) System.out.print("|");
			}
			System.out.println();
			for(int i = 0; i <= p.n; i++){
				if(p.p[i] != null)
					if (i < p.n) System.out.println("  Esq ("+ p.r[i].getCodTexto()+"): " );
					else System.out.println("  Dir ("+ p.r[i-1].getCodTexto()+"): " );
				imprime(p.p[i], nivel + 1);
			}
		}
	}
	
	public void imprime(){
		System.out.println("ÁrvoreB:");
		this.imprime(this.raiz, 0);
	}
}
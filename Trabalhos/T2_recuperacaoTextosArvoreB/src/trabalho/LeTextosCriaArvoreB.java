package trabalho;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class LeTextosCriaArvoreB{
	
	public static void main(String[] args) throws Exception{
		
		System.out.printf(":::TRABALHO DE AEDII\n:::ICT UNIFESP\n:::Renata Sendreti Broder\n\n\nÁRVORE B\n\n");
		ArvoreB arvore = new ArvoreB(2);
		String caminho = "stopwords.txt";
		arvore.criarListaStopWords(caminho);
		
		Scanner teclado = new Scanner(System.in);
		System.out.println("Digite a quantidade [1, 32] de textos que serão inseridos na árvore: ");
		int N = teclado.nextInt();
		System.out.print("Como você deseja inserir os textos na árvore?\n    Ordem natural (de 1 até "+N+") (1)\n    Manualmente (2)\n");
		int op = teclado.nextInt();
		
		String auxTit;
		
		if(op == 1){
			System.out.println("Inserindo em ordem...");
			for(int i=0; i<N; i++){
				HashMap<String, Integer> auxVF = new HashMap<String, Integer>();
				int[] repetido = new int[1];
				if(i<9) caminho = "texto0"+(i+1)+".txt";
				else caminho = "texto"+(i+1)+".txt";
				try{
					FileReader arquivo = new FileReader(caminho);
					BufferedReader lerArq = new BufferedReader(arquivo);
	
					String linha = lerArq.readLine(); // le primeira linha. EOF: linha == null.
					auxTit = linha; //salva o titulo que esta na primeira linha
					linha = linha.replaceAll("[,.:;!?\"/()—-—–•“”‘’]",""); //retira caracteres especiais
					
					do{
						String minusculo = linha.toLowerCase();//converte tudo para minusculo
	
						minusculo = minusculo.replaceAll("[,.:;!?\"/()—-—–•“”‘’]",""); //retira caracteres especiais
						//System.out.println(minusculo); //imprime para verificar se esta deixando minusculo e sem carac especiais
						StringTokenizer textoMin = new StringTokenizer(minusculo);
						
						while(textoMin.hasMoreTokens()){
							String palavra = textoMin.nextToken();
							Integer freq = auxVF.get(palavra);
							if(!arvore.getStopwords().contains(palavra)){
								if (freq != null)
									auxVF.put(palavra, freq+1);
								else
									auxVF.put(palavra, 1);
							}
						}
						linha = lerArq.readLine(); // le da segunda ate a ultima linha
					}while(linha != null);
					arquivo.close();
					
					HashMap<String, Integer> aux = new HashMap<String, Integer>();
					
					for(Map.Entry<String,Integer> pair : auxVF.entrySet()){
					    if(pair.getValue() >= 3)
					    	aux.put(pair.getKey(), pair.getValue());
					}
					
					MeuItem item = new MeuItem((i+1), auxTit, caminho, aux);
					repetido = arvore.insere(item, repetido);
				}
				catch (IOException e){
				System.err.printf("Erro na abertura do arquivo: %s.\n", e.getMessage());
				}
			}
		}
		else if(op == 2){
			System.out.println("Você optou por inserir manualmente.");
			for(int i=0; i<N; i++){
				HashMap<String, Integer> auxVF = new HashMap<String, Integer>();
				int[] repetido = new int[1];
				System.out.println("Digite o código do "+(i+1)+"o. texto a ser inserido:");
				int t = teclado.nextInt();
				if(i<9) caminho = "texto0"+(t+1)+".txt";
				else caminho = "texto"+(t+1)+".txt";
				try{
					FileReader arquivo = new FileReader(caminho);
					BufferedReader lerArq = new BufferedReader(arquivo);
	
					String linha = lerArq.readLine(); // le primeira linha. EOF: linha == null.
					linha = linha.replaceAll("[,.:;?\"/()—-—–•“”]",""); //retira caracteres especiais
					auxTit = linha; //salva o titulo que esta na primeira linha
					
					do{
						String minusculo = linha.toLowerCase();//converte tudo para minusculo
	
						minusculo = minusculo.replaceAll("[,.:;?\"//()—-—–•“”‘’]",""); //retira caracteres especiais
						//System.out.println(minusculo); //imprime para verificar se esta deixando minusculo e sem carac especiais
						StringTokenizer textoMin = new StringTokenizer(minusculo);
						
						while(textoMin.hasMoreTokens()){
							String palavra = textoMin.nextToken();
							Integer freq = auxVF.get(palavra);
							if(!arvore.getStopwords().contains(palavra)){
								if (freq != null)
									auxVF.put(palavra, freq+1);
								else
									auxVF.put(palavra, 1);
							}
						}
						linha = lerArq.readLine(); // le da segunda ate a ultima linha
					}while(linha != null);
					arquivo.close();
					
					HashMap<String, Integer> aux = new HashMap<String, Integer>();
					
					for(Map.Entry<String,Integer> pair : auxVF.entrySet()){
					    if(pair.getValue() >= 3)
					    	aux.put(pair.getKey(), pair.getValue());
					}
					
					MeuItem item = new MeuItem((t+1), auxTit, caminho, aux);
					repetido = arvore.insere(item, repetido);
					if(repetido[0] == 1) i--;
				}
				catch (IOException e){
					i--;
					System.err.printf("Erro na abertura do arquivo: %s.\n", e.getMessage());
				}
			}
		}
		
		while(true){
			if (arvore.raizEhNull()){
				System.out.println("Não há mais elementos na árvore! Encerrando o programa.");
				break;
			}
			System.out.printf("\nO que você deseja fazer agora?\n    Imprimir a árvore (1)\n    Realizar uma query (2)\n    Remover um texto pelo código (3)\n    Buscar e abrir um texto pelo código (4)\n    Pressione qualquer outra opção numérica para sair.\n");
			try{
				op = teclado.nextInt();
			
				if(op==1) arvore.imprime();
				else if(op == 2){
					System.out.println("Digite a palavra a ser buscada:");
					String palavra = teclado.next();
					if(!arvore.getStopwords().contains(palavra)) arvore.query(palavra.toLowerCase());
					else System.out.println("A palavra digitada é uma stopword.");
				}
				else if(op == 3){
					System.out.println("Digite o código do texto a ser removido:");
					MeuItem r = new MeuItem(teclado.nextInt());
					arvore.remove(r);
					System.out.println("O texto cujo código é "+r.getCodTexto()+" foi removido.");
				}
				else if(op == 4){
					System.out.println("Digite o código do texto que você quer buscar:");
					MeuItem item = new MeuItem(teclado.nextInt());
					item = (MeuItem) arvore.busca(item);
					if(item == null) System.out.println ("Item nao encontrado");
					else{
						System.out.println ("Item encontrado");
						java.awt.Desktop.getDesktop().open(new File(item.getCaminhoTexto()));
					}
				}
				else break;
			}
			catch (InputMismatchException e){
				System.out.println("Opção inválida");
				teclado.nextLine();
			}
		}
		
		teclado.close();
	}
}

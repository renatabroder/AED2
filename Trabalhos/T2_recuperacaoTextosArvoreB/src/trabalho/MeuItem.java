package trabalho;
import java.io.*;
import java.util.HashMap;

public class MeuItem{
	
	private int codTexto;
	private String tituloTexto;
	private String caminhoTexto;
	private HashMap<String, Integer> vetorFreq;
	
	public MeuItem(int codigo){
		this.codTexto = codigo;
	}
	
	public MeuItem(int codigo, String tit, String cam, HashMap<String, Integer> vf){
		this.codTexto = codigo;
		this.tituloTexto = tit;
		this.caminhoTexto = cam;
		this.vetorFreq = vf;
	}
	
	public int getCodTexto() {
		return codTexto;
	}

	public String getTituloTexto() {
		return tituloTexto;
	}

	public String getCaminhoTexto() {
		return caminhoTexto;
	}

	public HashMap<String, Integer> getVetorFreq() {
		return vetorFreq;
	}

	public int compara(MeuItem it){
		MeuItem item = (MeuItem) it;
		if (this.codTexto < item.codTexto) return -1;
		else if (this.codTexto > item.codTexto) return 1;
		return 0;
	}

	public void alteraCodTexto(Object codTexto){
		Integer cod = (Integer) codTexto;
		this.codTexto = cod.intValue ();
	}

	public Object recuperaCodTexto(){
		return new Integer (this.codTexto);
	}

	public String toString(){
		return "| CodTexto: " + this.codTexto + " ("+ this.tituloTexto + ") " /*+ vetorFreq*/;
	}

	public void gravaArq (RandomAccessFile arq) throws IOException{
		arq.writeInt (this.codTexto);
	}

	public void leArq(RandomAccessFile arq) throws IOException{
		this.codTexto = arq.readInt();
	}

	public static int tamanho(){
		return 4;
	}
}
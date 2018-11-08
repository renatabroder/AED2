import numpy as np
import matplotlib.pyplot as plt

import sys
listaArgs = sys.argv

print('Gerando graficos...')
#VeiculosUF.txt TotalAnos.txt VeiculosCategoria.txt MaiorCategUF.txt


# Lendo e organizando dados por estado
arquivo = open("VeiculosUF.txt", 'r') 
linhas = arquivo.readlines()
arquivo.close()

UFsNO = ['AM', 'RR', 'AP', 'PA', 'TO', 'RO', 'AC']
UFsNE = ['MA', 'PI', 'CE', 'RN', 'PE', 'PB', 'SE', 'AL', 'BA']
UFsCO = ['MT', 'MS', 'GO']
UFsSW = ['SP', 'RJ', 'ES', 'MG']
UFsS = ['PR', 'RS', 'SC']
dicionarioRegioes = {'Norte':0, 'Nordeste':0, 'Centro-Oeste':0, 'Sudeste':0, 'Sul':0} # Cria dicionario de regioes

listaUfs = []
listaValores = []
for line1 in linhas: # Separa dados do arquivo em duas listas e soma valores das regioes
    siglaUf = line1.split(' ')[0] # Separa dados das linhas por ' '
    valorUf = line1.split(' ')[1]
    listaUfs.append(siglaUf) # Adiciona UF do arquivo na lista
    listaValores.append(int(valorUf)/1000000) # Adiciona valores quantitativos de veiculos na lista
    
    if (siglaUf in UFsNO): dicionarioRegioes['Norte'] += int(valorUf)/1000000 # Altera valores regionais no dicionario
    elif (siglaUf in UFsNE): dicionarioRegioes['Nordeste'] += int(valorUf)/1000000
    elif (siglaUf in UFsCO): dicionarioRegioes['Centro-Oeste'] += int(valorUf)/1000000
    elif (siglaUf in UFsSW): dicionarioRegioes['Sudeste'] += int(valorUf)/1000000
    elif (siglaUf in UFsS): dicionarioRegioes['Sul'] += int(valorUf)/1000000
    
total = dicionarioRegioes['Norte'] + dicionarioRegioes['Nordeste'] + dicionarioRegioes['Centro-Oeste'] + dicionarioRegioes['Sudeste'] + dicionarioRegioes['Sul'] #total geral somou

# Gráfico de Barras (VeiculosUF)
fig = plt.figure(figsize=(10,6)) # Cria uma imagem do tamanho definido
barra = fig.add_subplot(111) # Adiciona um grafico DE BARRAS

ind = np.arange(len(listaUfs)) # Espacamento entre barras de estados
width = 0.4 # Largura da barra

lim = int(max(listaValores)) 
lim = int(lim/100)
lim = lim + 1
lim = lim * 100 # Limite superior do valor (teto em escala de milhoes)

barra.set_xlim(-2*width,len(ind)) # Limites laterais
barra.set_ylim(0, lim) # Limites inferior e superior
barra.set_ylabel('Valor total de veiculos (em milhoes)') # Texto lateral
plt.title('Veiculos por UF (2012-2015)', y=1.075, x=0.815, bbox={'facecolor':'0.8', 'pad':5}) # Texto superior
barra.set_xticks(ind) # Espacamento entre barras (ind la em cima)
xtickNames = barra.set_xticklabels(listaUfs) # Unidades inferiores (estados)
plt.setp(xtickNames, rotation=0, fontsize=10) # Insercao dos textos inferiores
rects1 = barra.bar(ind, listaValores, width, color="green") # Insercao das barras
barra.set_position([0.1, 0.1, 0.85, 0.75]) #[left, bottom, width, height] Tamanho e posicao do grafico
plt.savefig(('barras_' + 'VeiculosUF.txt').replace('txt', 'png'), dpi=300) #salva grafico em imagem png

# Gráfico de Pizza (VeiculosUF)
srtDic = dicionarioRegioes.items()
srtDic = sorted(srtDic, key=lambda x: x[1], reverse = True) #tuplas ordenados por valor, pra nao ficar feio, um pequeninho no meio de dois grandes

lbls = srtDic[0][0], srtDic[1][0], srtDic[2][0], srtDic[3][0], srtDic[4][0] #label = primeira informacao do tuple segundo [0] = indice da coisa que a gente quer no strdic
sizes = [srtDic[0][1]/total, srtDic[1][1]/total, srtDic[2][1]/total, srtDic[3][1]/total, srtDic[4][1]/total] #tamanhos dos arcos = porcentagem dos valores por regiao
expl = (0, 0, 0, 0, 0) #Parte com maior valor destacada

fig1, pizza = plt.subplots()
pizza.pie(sizes, explode=expl, labels=lbls, autopct='%1.1f%%', pctdistance=0.8) #autopct = exibir porcentagens, pctdistance = distancia das porcentagens em relacao ao centro
pizza.axis('equal') #Grafico centralizado

plt.title('Veiculos/Regiao (2012-15)', y=.955, x=0.85, bbox={'facecolor':'0.8', 'pad':5}) #texto superior
plt.savefig(('pizza_' + 'VeiculosUF.txt').replace('txt', 'png'), dpi=300) #salva grafico em imagem png




# Lendo e organizando dados por ano
arquivo2 = open("TotalAnos.txt", 'r')
linhas2 = arquivo2.readlines()
arquivo2.close()

listaAnos = []
listaValoresAno = []
for line2 in linhas2:  # Separa dados do arquivo em duas listas
    ano = line2.split(' ')[0] # Separa por ' '
    valorAno = line2.split(' ')[1]
    listaAnos.append(ano) # Adiciona ano do arquivo na lista 
    listaValoresAno.append(int(valorAno)/1000000) # Adiciona valores quantitativos de veiculos na lista

# Gráfico de Barras (TotalAnos)
fig = plt.figure(figsize=(10,6)) # Cria uma imagem do tamanho definido
barra = fig.add_subplot(111) # Adiciona um grafico DE BARRAS

ind = np.arange(len(listaAnos)) # Espacamento entre barras de estados
width = 0.4 # Largura da barra

lim = int(max(listaValoresAno)) 
lim = int(lim/100)
lim = lim + 1
lim = lim * 100 # Limite superior do valor

barra.set_xlim(-2*width,len(ind)) # Limites laterais
barra.set_ylim(0, lim) # Limites inferior e superior
barra.set_ylabel('Quantidade de veiculos (em milhoes)') # Texto lateral
plt.title('Veiculos por ano', y=1.075, x=0.815, bbox={'facecolor':'0.8', 'pad':5}) # Texto superior
barra.set_xticks(ind) # Espacamento entre barras (ind la em cima)
xtickNames = barra.set_xticklabels(listaAnos) # Unidades inferiores (anos)
plt.setp(xtickNames, rotation=0, fontsize=10) # Insercao dos textos inferiores em vertical
rects1 = barra.bar(ind, listaValoresAno, width, color="green") # Insercao das barras
barra.set_position([0.1, 0.1, 0.85, 0.75]) #[left, bottom, width, height] Tamanho e posicao do grafico
plt.savefig(('barras_' + "TotalAnos.txt").replace('txt', 'png'), dpi=300) #salva grafico em imagem png





# Lendo e organizando dados por categoria (VeiculosCategoria)
arquivo3 = open("VeiculosCategoria.txt", 'r') #Ler arquivo
linhas3 = arquivo3.readlines()
arquivo3.close()

totalDic = 0
dicioCateg = {}
dicioCateg['Outros'] = 0
for line in linhas3: #Soma os valores por regiao e separa as informacoes do arquivo em 2 listas
    categoria = line.split(' ')[0] #separa por ' ' [0] = primeira palavra
    valorCat = line.split(' ')[1]
    if(categoria == 'Onibus' or categoria == 'Caminhao-trator' or categoria == 'Utilitario' or categoria == 'Micro-onibus' or categoria == 'Trator-de-rodas' or categoria == 'Outros'):
        dicioCateg['Outros'] += float(valorCat)/1000000
    else:
        dicioCateg[categoria] = float(valorCat)/1000000
    totalDic += float(valorCat)/1000000

# Gráfico de Pizza (VeiculosCategoria)
srtDic = dicioCateg.items()
srtDic = sorted(srtDic, key=lambda x: x[1], reverse = True) #tuplas ordenados por valor, pra nao ficar feio, um pequeninho no meio de dois grandes

lbls = [srtDic[0][0], srtDic[1][0], srtDic[2][0], srtDic[3][0], srtDic[4][0], srtDic[5][0], srtDic[6][0]] #label = primeira informacao do tuple segundo [0] = indice da coisa que a gente quer no strdic
sizes = [srtDic[0][1]/totalDic, srtDic[1][1]/totalDic, srtDic[2][1]/totalDic, srtDic[3][1]/totalDic, srtDic[4][1]/totalDic, srtDic[5][1]/totalDic, srtDic[6][1]/totalDic] #tamanhos dos arcos = porcentagem dos valores por regiao
expl = (0, 0, 0, 0, 0, 0, 0) # Parte com maior valor destacada

fig1, pizza = plt.subplots()
pizza.pie(sizes, explode=expl, labels=lbls, autopct='%1.1f%%', pctdistance=0.8) #autopct = exibir porcentagens, pctdistance = distancia das porcentagens em relacao ao centro
pizza.axis('equal') # Grafico centralizado

plt.title('Veiculos/Categoria (2012-15)', y=.955, x=0.85, bbox={'facecolor':'0.8', 'pad':5}) # Texto superior
plt.savefig(('pizza_' + "VeiculosCategoria.txt").replace('txt', 'png'), dpi=300) # Salva grafico em imagem png






# Lendo e organizando dados por tipo de veiculo em maior quantidade
arquivo4 = open("MaiorCategUF.txt", 'r') #Ler arquivo
linhas4 = arquivo4.readlines()
arquivo4.close()

listaUfs = []
listaValoresMaior = []
for line4 in linhas4: #Somatiza os valores por regiao e separa as informacoes do arquivo em 2 listas

    siglaUf = line4.split(' ')[0] #separa por ' ' [0] = primeira palavra
    categoriaUf = line4.split(' ')[1]
    valorCatUf = line4.split(' ')[2]
    listaUfs.append(siglaUf + " " + categoriaUf) #lendo as UFs do arquivo 
    listaValoresMaior.append(int(valorCatUf)/1000000) #lendo as quantidades de veiculos do arquivo



# Gráfico de Barras (MaiorCategUF)
fig = plt.figure(figsize=(10,8)) #criando uma imagem deste tamanho
barra = fig.add_subplot(111) #adicionando um grafico DE BARRAS

ind = np.arange(len(listaUfs)) #Espacamento entre barras de estados len
width = 0.4 #Largura da barra

lim = int(max(listaValoresMaior)) 
lim = int(lim/100)
lim = lim + 0.7
lim = (lim * 100) #Limite superior do valor

barra.set_xlim(-2*width,len(ind)) # Limites laterais
barra.set_ylim(0, lim) # Limites inferior e superior
barra.set_ylabel('Valor total de veiculos (em milhoes)') # Texto lateral
plt.title('Categoria de Veiculo em maior quantidade por estado (2012-2015)', y=1.075, x=0.650, bbox={'facecolor':'0.8', 'pad':5}) #texto superior
barra.set_xticks(ind) # Espacamento entre barras ind la em cima
xtickNames = barra.set_xticklabels(listaUfs) # Unidades inferiores (estados)
plt.setp(xtickNames, rotation=90, fontsize=7) # Insercao dos textos inferiores em vertical
rects1 = barra.bar(ind, listaValoresMaior, width, color="green") # Insercao das barras
barra.set_position([0.1, 0.14, 0.85, 0.69]) #[left, bottom, width, height] Tamanho e posicao do grafico
plt.savefig(('barras_' + "MaiorCategUF.txt").replace('txt', 'png'), dpi=300) #salva grafico em imagem png

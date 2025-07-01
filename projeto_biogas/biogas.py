import requests
import mysql.connector 
import time

def coletar_dados():
    valores_dados = []
    dados = requests.get("")
    dados_api = dados.json

    for item in dados_api:
        mensagem = item['message']
        partes = mensagem.split(',')
        parte_por_milhao = partes[0].split('=')[1].strip()
        valor_porcento = partes[1].split('=')[1].strip()
        valores_dados.append({'parte_por_milhao': parte_por_milhao, 'porcentagem': valor_porcento})

    #for i, valor in enumerate(valores_dados, start=1):
     #   print(f'Dados {i}: Parte por milhão = {valor['parte_por_milhao']}, Porcentagem = {valor['valor_porcentagem']}')

def inserir_dados(data):
    conn = mysql.connector.connect(
        host ='mysql',
        mysql_user = 'biogas',
        mysql_password = 'biogas123',
        mysql_database = 'biogas_db_name' )
    cursor = conn.cursor()
    for item  in data:
        sql = 'INSERT INTO biogas (parte_por_milhao, porcentagem) VALUES (%s, %s)'
        valor = (item['parte_por_milhao'], item['porcentagem'])
        cursor.execute(sql, valor)

    conn.commit()
    cursor.close()
    conn.close()

while True: 
    dados = coletar_dados()
    inserir_dados(dados)
    time.sleep(3600)

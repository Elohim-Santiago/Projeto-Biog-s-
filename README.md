 Monitoramento de Metano com ESP32, Python e Docker

Este projeto tem como objetivo monitorar a concentração de metano utilizando um microcontrolador **ESP32**, o sensor de gás **MQ-5**, uma aplicação **Python** e um banco de dados **MySQL**. Toda a solução é executada em containers usando **Docker**, o que facilita a implantação, o gerenciamento e a escalabilidade.

---

 📐 Arquitetura da Solução

O sistema é composto por três principais componentes:

- **ESP32 (firmware)**: lê a concentração de metano e envia os dados via HTTP para uma API.
- **Aplicação Python (`biogas.py`)**: coleta os dados da API e armazena no banco de dados.
- **Docker Compose**: orquestra os containers do MySQL, da aplicação Python e do Adminer (gerenciador web do banco).

---

🔧 Componentes

 1. Firmware do ESP32 (Arduino IDE)

- **Leitura do Sensor**: Captura valores analógicos do MQ-5 pelo pino 34.
- **Conversão**: Converte os dados para estimativas em **PPM** e **porcentagem**.
- **Conexão Wi-Fi**: Conecta a uma rede especificada.
- **Envio de Dados**: A cada 15 segundos, os dados são enviados via **HTTP POST** para `http://laica.ifrn.edu.br/access-ng/log/` no formato JSON.

---

2. Script Python - `biogas.py`

Responsável por:

- Recuperar os dados da API.
- Extrair os campos `parte_por_milhao` e `porcentagem`.
- Inserir os dados no banco MySQL (`biogas_db_name`, tabela `biogas`).
- Executar continuamente a cada **1 hora** (`3600` segundos).

---

3. Docker Compose (`docker-compose.yml`)

Define e inicia os seguintes serviços:

- **`mysqlsrv`**:
  - Imagem: `mysql:5.7`
  - Porta: `3306`
  - Banco: `biogas`
  - Senha: `biogas123`
  - Volume persistente: `/data/mysql`

- **`biogas_py`**:
  - Baseado em um `Dockerfile.c2`
  - Roda o `biogas.py`
  - Depende do `mysqlsrv`

- **`adminer`**:
  - Interface web para acessar o banco
  - Porta: `8080`

Todos os serviços estão na mesma **rede Docker bridge**: `biogas_integrador`.

---

## 🚀 Como Executar

### Pré-requisitos

- [ ] Docker e Docker Compose instalados
- [ ] Arduino IDE configurado para ESP32
- [ ] Placa ESP32
- [ ] Sensor MQ-5

### Passos

#### 1. Gravar o Firmware

- Abra o código na Arduino IDE.
- Configure `SECRET_SSID` e `SECRET_PASS`.
- Conecte o MQ-5 ao pino 34.
- Faça o upload para o ESP32.

#### 2. Estrutura de Diretórios

Crie a pasta do projeto:

projeto_biogas/
├── biogas.py
├── Dockerfile.c2
├── docker-compose.yml

3. Dockerfile.c2

```Dockerfile
FROM python:3.9-slim-buster
WORKDIR /app
COPY biogas.py .
RUN pip install requests mysql-connector-python
CMD ["python", "biogas.py"]
4. Subir os Containers
No terminal, no diretório onde está o docker-compose.yml:

docker-compose up -d
🌐 Acessando o Adminer
Acesse no navegador: http://localhost:8080

Sistema: MySQL

Servidor: mysqlsrv

Usuário: biogas

Senha: biogas123

Banco de dados: biogas_db_name

📊 Verificação dos Dados
O ESP32 envia os dados para a API.

O biogas.py coleta e insere no banco MySQL.

Os dados podem ser visualizados pela interface do Adminer.

💡 Melhorias Futuras
Definir o endpoint da API no biogas.py (requests.get("")) com o endereço real.

Criar uma API Flask/FastAPI se desejar que o ESP32 envie dados diretamente para o Python.

Adicionar validações e tratamento de erros.

Usar variáveis de ambiente para armazenar credenciais sensíveis.

Garantir que a tabela biogas esteja criada no banco com as colunas esperadas: id, parte_por_milhao, porcentagem, timestamp.

Sincronizar o tempo no ESP32, para registros precisos.


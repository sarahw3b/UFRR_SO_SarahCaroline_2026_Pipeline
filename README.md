# Programação Concorrente com Threads em Pipeline (Multistage)

Por: Sarah Caroline Amaral Pereira.

## Introdução

A programação concorrente é um paradigma fundamental na computação moderna, permitindo a execução de múltiplas tarefas de forma sobreposta no tempo. Esse modelo é amplamente utilizado para melhorar o desempenho e a eficiência de sistemas, especialmente em aplicações que envolvem processamento contínuo de dados.

Neste contexto, o modelo de pipeline multistage se destaca como uma abordagem eficaz para estruturar aplicações concorrentes. Nesse modelo, uma tarefa é dividida em etapas menores, permitindo que diferentes partes do processamento ocorram simultaneamente.

---

## Estrutura do Pipeline

O sistema desenvolvido é composto por três estágios principais:

- Leitura (produção de dados)
- Processamento (transformação dos dados)
- Escrita (consumo dos resultados)

Cada estágio é executado por uma thread independente, formando um fluxo contínuo de dados:

Leitura → Processamento → Escrita

A comunicação entre os estágios é realizada por meio de buffers intermediários.

---

## Concorrência e Paralelismo

A execução do pipeline caracteriza um sistema concorrente, no qual múltiplas tarefas progridem simultaneamente.

- Concorrência: múltiplas tarefas progridem de forma intercalada no tempo.
- Paralelismo: múltiplas tarefas são executadas simultaneamente em diferentes núcleos.

No sistema implementado, a concorrência está sempre presente, enquanto o paralelismo depende da arquitetura do hardware.

---

## Buffers e Desacoplamento

Os buffers são estruturas responsáveis por armazenar temporariamente os dados entre os estágios do pipeline.

Sua principal função é promover o desacoplamento entre as etapas, permitindo que cada thread opere de forma independente.

Isso possibilita:
- execução em ritmos diferentes entre os estágios;
- redução de bloqueios diretos;
- maior eficiência do sistema.

---

## Problemas de Concorrência

O acesso concorrente aos buffers pode gerar problemas como:

- Condição de corrida (race condition): ocorre quando múltiplas threads acessam um recurso compartilhado simultaneamente, gerando inconsistência de dados.
- Deadlock: ocorre quando threads ficam bloqueadas esperando umas pelas outras.
- Starvation: ocorre quando uma thread não consegue acesso ao recurso por longos períodos.

---

## Mecanismos de Sincronização

Para garantir a consistência do sistema, foram utilizados mecanismos de sincronização:

### Mutex

O mutex garante exclusão mútua, permitindo que apenas uma thread por vez acesse a região crítica (buffer).

### Variáveis de Condição

As variáveis de condição controlam quando uma thread pode acessar o buffer.

Elas permitem que a thread:
- espere quando o buffer está vazio;
- espere quando o buffer está cheio;
- seja acordada quando a condição muda.

---

## Modelo Produtor-Consumidor

O sistema segue o padrão produtor-consumidor:

- Leitura atua como produtora;
- Processamento atua como produtora e consumidora;
- Escrita atua como consumidora.

Esse modelo organiza o fluxo de dados entre os estágios.

---

## Comportamento do Sistema

Devido à natureza concorrente, a ordem de execução não é determinística. Isso significa que a saída pode variar entre execuções.

Esse comportamento é esperado e resulta do escalonamento das threads pelo sistema operacional.

Também pode ocorrer acúmulo de dados nos buffers, indicando a presença de gargalos quando um estágio é mais lento que os demais.

---

## Modelagem com Rede de Petri

<img width="733" height="271" alt="image" src="https://github.com/user-attachments/assets/4d8d3714-ca45-49ca-a904-3008a215873b" />

A Rede de Petri foi utilizada para representar o sistema de forma visual.

Foi usada na implementação da RP a aplicação 'Snoopy'.

Na modelagem:

- Lugares representam os buffers;
- Transições representam os estágios do pipeline;
- Tokens representam os dados.

Essa representação permite visualizar o fluxo de dados e identificar aspectos concorrentes do sistema.

---

## Conclusão

A implementação do pipeline com threads demonstra conceitos fundamentais de programação concorrente, como sincronização, comunicação entre threads e controle de acesso a recursos compartilhados.

O uso de buffers e mecanismos de sincronização garante a consistência do sistema, enquanto a estrutura em pipeline melhora o desempenho e a utilização dos recursos.

A modelagem com Rede de Petri complementa a análise, facilitando a compreensão do comportamento concorrente do sistema.

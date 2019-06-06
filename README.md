Este trabalho foi desenvolvido por Pablo Diego Silva da Silva(15/0019475) e Rodrigo Oliveira Campos(15/0021399).

Do protocolo padronizado temos o recebimento e envio de mensagens, broadcasts e listagem de usuários.

Mensagens:
    sender:recipient:message
Broadcast:
    sender:all:message
Comandos:
    list

Problemas conhecidos:
    - Estamos utilizando o mesmo stream de IO para recebimento e envio de mensagens, então caso eu receba uma mensagem enquanto estou digitando, elas irão se misturar e não vou conseguir enviar, tendo que reescrever o texto inicial.

    - É necessário escrever seu username ao enviar uma mensagem.

    - Não é um problema. Porém, como é necessário escrever o username ao enviar uma mensagem, eu consigo me passar por outro usuário, te enviando uma mensagem em um outro username que esteja disponível.


 Dificuldades de implementação do modelo de threads:
    Durante o desenvolvimento das threads, a thread específica para envio acabava ficando em um loop infinito gastando processamento. Deu trabalho, mas resolvemos utilizando um semáforo que desbloqueia a thread quando há uma mensagem a ser enviada.

//definição de blibliotecas
#include <WiFi.h>
#include <HTTPClient.h>

//definição de váriaveis
int jogada_1 = 1;
int jogada_2 = 1;
int led = 10;
int botao_p1 = 16;
int botao_p2 = 17;
int botao_confirm = 35;
int valor_rodada = 2;
int placar_p1 = 0;
int placar_p2 = 0;
int vez = 0;
int placar_max = 0;
int gamemode = 0;
int continuar = 1;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;
String header;


// definição de rede
const char *ssid = "Inteli-COLLEGE"; //no inteli usar : Inteli-COLLEGE
const char *password = "QazWsx@123"; // no inteli usar: QazWsx@123
WiFiServer server(80);

//definição de saidas e inputs
void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(botao_p1,INPUT_PULLUP);
  pinMode(botao_p2, INPUT_PULLUP);
  pinMode(botao_confirm, INPUT_PULLUP);
  // conexão ao wifi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address e inicio do servidor web
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  ////////
}

//função para trucar, aumentando o valor da rodada
void truco(){
  //desliga o led 
  digitalWrite(led, LOW);
//se o player não tiver trucado ele pode trucar
if (digitalRead(botao_p1) == LOW && vez != 1){
    valor_rodada +=2;
    vez = 1;
    tone (3, 1000, 300);
}
if (digitalRead(botao_p2) == LOW && vez != 2){
    valor_rodada +=2;
    vez = 2;
    tone (3, 1000, 300);
}
//após passar todos os trucos vai para contagem dos pontos
if (digitalRead(botao_confirm) == LOW){
    vez = 0;
    tone (3, 1000, 300);
    delay(500);
    tone (3, 1000, 300);
    gamemode =4;
}
//se atingir a pontuação maxima vai para contagem dos pontos
placar_max = valor_rodada + placar_p1;
if (placar_max == 12){
    vez = 0;
    tone (3, 1000, 300);
    delay(500);
    tone (3, 1000, 300);
    gamemode =4;
}
placar_max = valor_rodada + placar_p2;
if (placar_max == 12){
    vez = 0;
    tone (3, 1000, 300);
    delay(5000);
    tone (3, 1000, 300);
    gamemode =4;
}
}

//função para computar a jogada dos playes, podendo ser 1 papel 2 tesoura 3 pedra
void jogadas(){
    //contabiliza a jogada
    digitalWrite(led, HIGH);
    if (digitalRead(botao_p1) == LOW){
        jogada_1 +=1;
        if(jogada_1 > 3){
            jogada_1 = 3;
        }
      delay(500);
    }
    if (digitalRead(botao_p2) == LOW){
        jogada_2 +=1;
            if(jogada_2 > 3){
            jogada_2 = 3;
            }
            delay(500);
    }
    //inicia a fase de truco
    if (digitalRead(botao_confirm) == LOW){
        delay(1500);
        tone (3, 1000, 500);
        gamemode =2;
    }
}

//função para definir em que fase o jogo está


//faz a transição entre as partes do game
void loop(){

///
//Criação do client
  WiFiClient client = server.available();
  if (client)
  {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client."); 
   String currentLine = ""; 
   //loop que acontece enquanto o client estiver ativo      
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { 
      currentTime = millis();
      //se o client estiver disponivel
      if (client.available())
      {                         
        char c = client.read(); 
        Serial.write(c);        
        header += c;
        if (c == '\n')
        { 
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

 client.println("<!DOCTYPE html>");
 client.println("<html>");
 client.println("<head>");
 client.println("<head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Document</title>");
     client.println("<title>Bem vindo ao pedra Papel ou tesoura Truco version</title>");
 client.println("</head>");
 client.println("<body>");
     client.println("<h1>Bem vindo ao pedra Papel ou tesoura Truco version</h1>");
 client.println("</body>");
 client.println("<div>");
     client.println("<h1>Informações do jogo:</h1>");
 client.println("</div>");
 client.println("</html>");



/////
while(continuar == 1){
while(gamemode == 0){
client.println("<h4>O jogo começou, cada rodada vale 2 pontos, quem fizer 12 primeiro ganha!</h4>");
client.println("<h4>não aperte para papel, 1 vez para tesoura 2 para pedra, quando quiser aperte o botão central para ir para o truco</h4>"); 
gamemode = 1 ; 
}
while(gamemode == 1){
jogadas();
}
while(gamemode == 2){
client.println("<h4>A fase de truco começou, aparte seu botão para trucar, aumentando o valor da rodada em 2 pontos, quando quiser aperte o botão central para contabilizar o resultado</h4>");
gamemode = 3;
}
while(gamemode == 3){
truco();
}
while(gamemode == 4){
  //desliga o led 
    digitalWrite(led, LOW);
  //situação de vitoria do jogador 1
    if (jogada_1 == 1 && jogada_2 == 3){
        placar_p1+= valor_rodada;
        client.println("<h4>Jogador 1 jogou papel</h4>");
        client.println("<h4>Jogador 2 jogou pedra</h4>");
       client.println("<h4>Jogador 1 venceu a rodada!</h4>");
    }
    digitalWrite(led, HIGH);
    delay(1000);
    if (jogada_1 == 2 && jogada_2 == 1){
        placar_p1+= valor_rodada;
        client.println("<h4>Jogador 1 jogou tesoura</h4>");
        client.println("<h4>Jogador 2 jogou papel</h4>");
        client.println("<h4>Jogador 1 venceu a rodada!</h4>");
    }
    digitalWrite(led, LOW);
    delay(1000);
    if (jogada_1 == 3 && jogada_2 == 2){
        placar_p1+= valor_rodada;
        client.println("<h4>Jogador 1 jogou pedra</h4>");
        client.println("<h4>Jogador 2 jogou tesoura</h4>");
        client.println("<h4>Jogador 1 venceu a rodada!</h4>");
    }
    digitalWrite(led, HIGH);
    delay(1000);
    //situação de vitoria do jogador 2
    if (jogada_2 == 1 && jogada_1 == 3){
        placar_p2+= valor_rodada;
        client.println("<h4>Jogador 1 jogou pedra</h4>");
        client.println("<h4>Jogador 2 jogou papel</h4>");
        client.println("<h4>Jogador 2 venceu a rodada!</h4>");
    }
    digitalWrite(led, LOW);
    delay(1000);
    if (jogada_2 == 2 && jogada_1 == 1){
        placar_p2+= valor_rodada;
        client.println("<h4>Jogador 1 jogou papel</h4>");
        client.println("<h4>Jogador 2 jogou tesoura</h4>");
        client.println("<h4>Jogador 2 venceu a rodada!</h4>");
    }
    if (jogada_2 == 3 && jogada_1 == 2){
        placar_p2+= valor_rodada;
        client.println("<h4>Jogador 1 jogou tesoura</h4>");
        client.println("<h4>Jogador 2 jogou pedra</h4>");
        client.println("<h4>Jogador 2 venceu a rodada!</h4>");
    }
    //situações de empate
    if (jogada_2 == 2 && jogada_1 == 2){
        client.println("<h4>Empate,ambos jogaram tesoura</h4>");
    }
        if (jogada_2 == 1 && jogada_1 == 1){
        client.println("<h4>Empate,ambos jogaram papel</h4>");
    }
        if (jogada_2 == 3 && jogada_1 == 3){
        client.println("<h4>Empate,ambos jogaram pedra</h4>");
    }
    //retorna os placares dos jogadores
    client.println("<h4>Placar do jogador 1: </h4>");
    client.println(placar_p1);
    client.println("<h4>Placar do jogador 2: </h4>");
    client.println(placar_p2);

    //reset das jogadas
    jogada_1 = 1;
    jogada_2 = 1;
   
   //se o player tiver o placar para ganhar
    if (placar_p1 == 12){
        client.println("<h4>Jogador 1 venceu o jogo!</h4>");
        client.println("<h4>Reiniciando o jogo...</h4><br><br><br><br>");
        placar_p1 = 0;
        placar_p2 = 0;
        valor_rodada =0;
        gamemode =0;
    }
    if (placar_p2 == 12){
        client.println("<h4>Jogador 2 venceu o jogo!</h4>");
        client.println("<h4>Reiniciando o jogo...</h4><br><br><br><br>");
        placar_p1 = 0;
        placar_p2 = 0;
        valor_rodada =0;
        gamemode =0;
    }
    //se não uma nova rodada se inicia
    else{
        client.println("<h4>nova rodada começando...</h4><br><br><br><br>");
        gamemode =0;
        valor_rodada =2;
    }
}
}
          }
        }
      }
    }
}
}

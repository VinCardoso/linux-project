/* Voltimetro com Arduino 0 - 5v */

int val;
double val_comp;
int select;

void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT); 
}

void loop() {
  
  select = 2;

  // Seleciona qual dos dois irá ler  
    if(select == 1){
      val = analogRead(A0); // Realiza a leitura no primeiro
    }else if(select == 2){
      val = analogRead(A1); // Realiza a leitura no primeiro
    }
  
  if(val != val_comp){ //Caso não seja repitido mostra
    Serial.print("Tensão: ");
    Serial.print(val);
    Serial.print("/1023 - ");
    Serial.print(val* 5.00/1023.00);
    Serial.println("V/5.00V");
    val_comp = val;
  }
  
  delay(100);
}
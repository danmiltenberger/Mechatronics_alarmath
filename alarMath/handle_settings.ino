
Settings modifySettings(Settings setn) {
  // change the different settings values, maybe cycling through a screen? 
  // right now, just show it was called and skip

  lcd.clear();
  lcd.print("Mod Set");  
  delay(1000);   // show for a second

  return setn;
}
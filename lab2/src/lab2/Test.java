package lab2;

import java.io.IOException;
import java.nio.charset.Charset;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.firefox.*;
import com.csvreader.CsvReader;

public class Test {
   
public static void main(String[] args) throws IOException {
                            
       CsvReader r = new CsvReader("D://天津大学//大三春—天津大学//软件测试//inputgit.csv", ',' , Charset.forName("GBK"));
       r.readHeaders();  //读头部
       while (r.readRecord()) {                        
    	   r.readRecord();             
    	   String number_csv = r.get("学号");              
    	   String name_csv = r.get("姓名");
    	   String address_csv = r.get("github地址");
    	   String pwd_csv = number_csv.substring(number_csv.length()-6,number_csv.length());
    	   //读取CSV文件中数据
       
    	   System.setProperty("webdriver.firefox.bin", "C:/Program Files (x86)/Mozilla Firefox/firefox.exe"); 
    	   WebDriver driver = new FirefoxDriver();
    	   driver.get("http://121.193.130.195:8080/");               
    	   driver.manage().window().maximize();
    	   //通过火狐浏览器打开网址
      
    	   WebElement input_name = driver.findElement(By.id("name"));
    	   WebElement input_pwd = driver.findElement(By.id("pwd"));
    	   WebElement btn = driver.findElement(By.id("submit"));
    	   input_name.clear();
    	   input_pwd.clear();
    	   input_name.sendKeys(number_csv);
    	   input_pwd.sendKeys(pwd_csv);  	   
    	   btn.click();
    	   //输入CSV中读出的数据
    	   
    	   String info = driver.findElement(By.xpath("//tbody[@id='table-main']")).getText();
    	   //通过Xpath来定位用户信息的位置
    	   String name = info.substring(info.indexOf("名") + 2, info.indexOf("学") - 1);
    	   String number = info.substring(info.indexOf("号") + 2, info.indexOf("G") - 1);
    	   String address = info.substring(info.indexOf("址") + 2);  
    	   //记录网页中的信息
    	   
    	   String firstname = name.substring(0, 1);
    	   if(name_csv.equals(name)&&number_csv.equals(number)&&address_csv.equals(address))
               System.out.println(firstname+"同学通过");
    	   else
               System.out.println(firstname+"同学未通过");
    	   driver.close();
        }
        r.close();         
}  
}
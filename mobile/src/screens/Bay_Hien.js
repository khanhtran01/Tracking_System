import React, {useState, useEffect} from 'react';
import {
  View,
  Text,
  SafeAreaView,
  Image,
  TouchableOpacity,
  StyleSheet,
  StatusBar,
  Dimensions,
} from 'react-native';
import {ScrollView} from 'react-native-gesture-handler';

var pWidth = Dimensions.get('window').width;
var pHeight = Dimensions.get('window').height;

export default function HomeScreen({navigation}) {
  const [isLoading, setLoading] = useState(true);
  const [data, setData] = useState([]);
  const [data2, setData2] = useState([]);
  //File minh se catch co data sau:
  // Data này thay đổi tuỳ theo ý muốn của mình, đây chỉ là số liệu ví dụ
  // "last_value": "{\"highest_temp\" : \"50\",
  //                 \"highest_temp_time\" : \"2022-05-08\",
  //                 \"total_car\" : \"34\",
  //                 \"most_car_month\" : \"04\"}",
  // nhu o duoi da access vao data.highest temp va no return 50
  // check feed cua ong Khanh de thu up len up xuong di
  // Đổi link ở trong cái fetch dưới và xem cái data có gì là đc
  const getData2 = async () => {
    try {
      const response = await fetch(
        'https://io.adafruit.com/api/v2/khanhtran01/feeds/humi-and-temp',
      );
      const json = await response.json();
      const json_fix = JSON.parse(json.last_value);
      setData2(json_fix);
    } catch (error) {
      console.error(error);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    getData2();
  }, []);

  const getData = async () => {
    try {
      const response = await fetch(
        'https://io.adafruit.com/api/v2/khanhtran01/feeds/statical',
      );
      const json = await response.json();
      const json_fix = JSON.parse(json.last_value);
      setData(json_fix);
    } catch (error) {
      console.error(error);
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    getData();
  }, []);
  //Den day la khuc lay data tu adafruit, data se nam trong data qua lenh setData

  return (
    <SafeAreaView style={styles.head}>
      <View style={styles.header}>
        <View style={styles.container}>
          <TouchableOpacity onPress={() => navigation.openDrawer()}>
            <Image
              style={styles.img}
              source={require('../assets/images/bar-menu.webp')}
            />
          </TouchableOpacity>
          <Text style={styles.text_header}>Ngã tư Bảy Hiền</Text>
        </View>
      </View>
      <ScrollView>
        <View style={styles.contentHome}>
          <View style={styles.present_temp}>
            <Text style={styles.text}>Nhiệt độ hiện tại</Text>
            <Text
              style={{
                fontSize: 60,
                textAlign: 'center',
                marginTop: 1,
                color: data2.temp > 30 ? 'red' : 'blue',
              }}>
              {data2.temp}*C
            </Text>
          </View>
          <View
            style={{
              height: 140,
              width: pWidth * 0.95 * 0.5 - 6,
              margin: 5,
              borderColor: '#003300',
              borderRadius: 15,
              justifyContent: 'center',
              backgroundColor: data2.temp >= 30 ? '#E55451' : 'white',
            }}>
            <Text
              style={{
                textAlign: 'center',
                fontSize: data2.temp > 29 ? 40 : 30,
                fontWeight: data2.temp > 29 ? 'bold' : 'normal',
                color: 'black',
              }}>
              Nóng
            </Text>
          </View>
          <View
            style={{
              height: 140,
              width: pWidth * 0.95 * 0.5 - 6,
              margin: 5,
              borderColor: '#003300',
              borderRadius: 15,
              justifyContent: 'center',
              backgroundColor: data2.temp < 30 ? '#33CCFF' : 'white',
            }}>
            <Text
              style={{
                textAlign: 'center',
                fontSize: data2.temp < 30 ? 40 : 30,
                fontWeight: data2.temp < 30 ? 'bold' : 'normal',
                color: 'black',
              }}>
              Mát
            </Text>
          </View>
          <View style={styles.badCar}>
            <Text style={styles.text}>Số xe vượt đèn đỏ</Text>
            <Text
              style={{
                fontSize: 60,
                textAlign: 'center',
                marginTop: 1,
                color: data.total_car > 30 ? 'red' : 'blue',
              }}>
              {data.total_car}
            </Text>
          </View>
          <View style={styles.highest_temp}>
            <Text style={styles.text2}>Nhiệt độ cao nhất </Text>
            <Text style={styles.text_highest_temp}>{data.highest_temp}*C</Text>
          </View>
          <View style={styles.total_car}>
            <Text style={styles.text2}>Tháng nhiều xe nhất</Text>
            <Text style={styles.text_total_car}>{data.most_car_month}</Text>
          </View>
        </View>
      </ScrollView>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  head: {
    flex: 1,
    backgroundColor: '#EFEFEF',
  },
  header: {
    height: pHeight * 0.08,
    backgroundColor: '#367BF5',
    width: pWidth,
    borderWidth: 3,
    alignItems: 'center',
    justifyContent: 'center',
    marginTop: StatusBar.currentHeight,
    marginBottom: 4,
  },
  container: {
    flexDirection: 'row',
    alignItems: 'center',
    justifyContent: 'center',
  },
  img: {
    flex: 1,
    width: 50,
    height: 50,
    marginLeft: 6,
  },
  text_header: {
    flex: 1,
    color: 'black',
    fontSize: 30,
    textAlign: 'center',
    fontWeight: 'bold',
  },
  contentHome: {
    flexDirection: 'row',
    flexWrap: 'wrap',
    alignContent: 'center',
    justifyContent: 'center',
    backgroundColor: '#EFEFEF',
  },
  present_temp: {
    height: 140,
    width: pWidth * 0.95,
    margin: 5,
    backgroundColor: 'white',
    borderRadius: 15,
  },
  text: {
    fontSize: 27,
    fontStyle: 'italic',
    marginLeft: 10,
    marginTop: 5,
    color: '#003300',
  },
  badCar: {
    height: 140,
    width: pWidth * 0.95,
    margin: 5,
    backgroundColor: 'white',
    borderRadius: 15,
  },
  highest_temp: {
    height: 140,
    width: pWidth * 0.95 * 0.5 - 6,
    margin: 5,
    backgroundColor: 'white',
    borderRadius: 15,
  },
  total_car: {
    height: 140,
    width: pWidth * 0.95 * 0.5 - 6,
    margin: 5,
    backgroundColor: 'white',
    borderRadius: 15,
  },
  text2: {
    fontSize: 18,
    fontStyle: 'italic',
    marginLeft: 10,
    marginTop: 5,
    color: '#003300',
  },
  text_highest_temp: {
    fontSize: 40,
    textAlign: 'center',
    color: 'green',
    marginTop: 19,
  },
  text_total_car: {
    fontSize: 40,
    textAlign: 'center',
    color: 'green',
    marginTop: 19,
  },
});

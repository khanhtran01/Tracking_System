import {
  StyleSheet,
  Text,
  View,
  Switch,
  TextInput,
  Alert,
  TouchableOpacity,
  SafeAreaView,
  Image,
  StatusBar,
} from 'react-native';
import React, {useState} from 'react';
import {Dimensions} from 'react-native';
import {ScrollView} from 'react-native-gesture-handler';

var pWidth = Dimensions.get('window').width;
var pHeight = Dimensions.get('window').height;

export default function Switches({navigation}) {
  const [isEnabled1, set1IsEnabled] = useState(false);
  const [isEnabled2, set2IsEnabled] = useState(false);
  const [isEnabled3, set3IsEnabled] = useState(false);

  const toggleSwitch1 = () => set1IsEnabled(previousState => !previousState);
  const toggleSwitch2 = () => set2IsEnabled(previousState => !previousState);
  const toggleSwitch3 = () => set3IsEnabled(previousState => !previousState);

  const [redTime, setRed] = useState('');
  const [yellowTime, setYellow] = useState('');
  const [greenTime, setGreen] = useState('');

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
          <Text style={styles.text_header}>Ngã tư Thủ Đức</Text>
        </View>
      </View>
      <ScrollView>
        <View style={styles.container2}>
          <View style={styles.timeinput_container}>
            <Text style={styles.title}> Thời gian đèn </Text>
            <View style={styles.time_container}>
              <Text style={styles.switch_text}>Đỏ: </Text>
              <View style={styles.inputView}>
                <TextInput
                  keyboardType="number-pad"
                  maxLength={3}
                  style={styles.TextInput}
                  onChangeText={redTime => setRed(redTime)}
                />
              </View>
            </View>
            <View style={styles.time_container}>
              <Text style={styles.switch_text}>Vàng: </Text>
              <View style={styles.inputView}>
                <TextInput
                  keyboardType="number-pad"
                  maxLength={3}
                  style={styles.TextInput}
                  onChangeText={redTime => setYellow(yellowTime)}
                />
              </View>
            </View>
            <View style={styles.time_container}>
              <Text style={styles.switch_text}>Xanh: </Text>
              <View style={styles.inputView}>
                <TextInput
                  keyboardType="number-pad"
                  maxLength={3}
                  style={styles.TextInput}
                  onChangeText={redTime => setGreen(greenTime)}
                />
              </View>
            </View>

            <TouchableOpacity
              style={styles.send_button}
              onPress={() => Alert.alert('Bạn chắc chứ?')}>
              <Text style={{color: '#FFFFFF'}}>CẬP NHẬT</Text>
            </TouchableOpacity>
          </View>

          <View style={styles.mode_container}>
            <Text style={styles.title}> Chế độ đèn </Text>
            <View style={styles.switch_container}>
              <Text style={styles.switch_text2}> Bình thường: </Text>
              <Switch
                style={{transform: [{scaleX: 2}, {scaleY: 2}]}}
                trackColor={{false: '#767577', true: '#81b0ff'}}
                thumbColor={isEnabled1 ? '#f4f3f4' : '#f4f3f4'}
                onValueChange={toggleSwitch1}
                value={isEnabled1}
              />
            </View>
            <View style={styles.switch_container}>
              <Text style={styles.switch_text2}> Đi Chậm: </Text>
              <Switch
                style={{transform: [{scaleX: 2}, {scaleY: 2}]}}
                trackColor={{false: '#767577', true: '#81b0ff'}}
                thumbColor={isEnabled2 ? '#f4f3f4' : '#f4f3f4'}
                onValueChange={toggleSwitch2}
                value={isEnabled2}
              />
            </View>
            <View style={styles.switch_container}>
              <Text style={styles.switch_text2}> Dừng: </Text>
              <Switch
                style={{transform: [{scaleX: 2}, {scaleY: 2}]}}
                trackColor={{false: '#767577', true: '#81b0ff'}}
                thumbColor={isEnabled3 ? '#f4f3f4' : '#f4f3f4'}
                onValueChange={toggleSwitch3}
                value={isEnabled3}
              />
            </View>
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
    backgroundColor: 'yellow',
    width: pWidth,
    borderWidth: 3,
    alignItems: 'center',
    justifyContent: 'center',
    marginTop: StatusBar.currentHeight,
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
  container2: {
    marginTop: 3,
    backgroundColor: '#EFEFEF',
    alignItems: 'center',
    marginBottom: 10,
  },
  mode_container: {
    marginTop: 4,
    flexDirection: 'column',
    backgroundColor: '#FFFFFF',
    height: 275,
    borderColor: '#000000',
    alignItems: 'center',
    width: pWidth * 0.95,
    borderRadius: 15,
  },
  timeinput_container: {
    marginTop: 7,
    marginBottom: 5,
    flexDirection: 'column',
    backgroundColor: '#FFFFFF',
    height: 350,
    borderColor: '#000000',
    alignItems: 'center',
    width: pWidth * 0.95,
    borderRadius: 15,
  },
  switch_container: {
    width: pWidth * 0.55,
    flexDirection: 'row',
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
    flex: 1,
  },
  time_container: {
    width: pWidth * 0.75,
    flexDirection: 'row',
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
    flex: 1,
  },
  title: {
    fontSize: 30,
    fontWeight: 'bold',
    marginTop: 8,
    marginBottom: 4,
    color: '#464646',
  },
  switch_text: {
    fontSize: 20,
    width: pWidth * 0.35,
  },
  switch_text2: {
    fontSize: 20,
    width: pWidth * 0.5,
  },
  inputView: {
    height: 50,
    width: 150,
    borderWidth: 2,
    alignItems: 'center',
    backgroundColor: '#EFEFEF',
    borderRadius: 10,
  },
  TextInput: {
    height: 50,
    flex: 1,
    marginLeft: 20,
    fontSize: 24,
    justifyContent: 'center',
    alignSelf: 'center',
  },
  send_button: {
    width: '30%',
    borderColor: '#000000',
    borderWidth: 2,
    height: 50,
    alignItems: 'center',
    justifyContent: 'center',
    marginTop: 10,
    marginBottom: 15,
    backgroundColor: '#367BF5',
    marginRight: '5%',
    alignSelf: 'flex-end',
    borderRadius: 10,
  },
});

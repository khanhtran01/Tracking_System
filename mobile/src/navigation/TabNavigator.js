import React from 'react';
import {createBottomTabNavigator} from '@react-navigation/bottom-tabs';

import HomeScreen from '../screens/Thu_Duc';
import Switches from '../screens/Switches';

import Ionicons from 'react-native-vector-icons/Ionicons';
import {Dimensions} from 'react-native';

var pHeight = Dimensions.get('window').height;

const Tab = createBottomTabNavigator();

const TabNavigator = () => {
  return (
    <Tab.Navigator
      screenOptions={{
        headerShown: false,
        tabBarInactiveTintColor: '#748c94',
        tabBarActiveTintColor: '#e32f45',
        tabBarStyle: {
          height: pHeight * 0.07,
        },
        tabBarIconStyle: {
          top: 2,
        },
        tabBarLabelStyle: {
          paddingBottom: 1,
        },
      }}>
      <Tab.Screen
        name="Home2"
        component={HomeScreen}
        options={{
          tabBarLabel: 'Statistic',
          tabBarIcon: ({color, size}) => {
            return <Ionicons name="stats-chart" size={size} color={color} />;
          },
        }}
      />
      <Tab.Screen
        name="Switches"
        component={Switches}
        options={{
          tabBarLabel: 'Switches',
          tabBarIcon: ({color, size}) => {
            return <Ionicons name="settings" size={size} color={color} />;
          },
        }}
      />
    </Tab.Navigator>
  );
};

export default TabNavigator;

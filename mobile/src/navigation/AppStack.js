import React from 'react';
import {createDrawerNavigator} from '@react-navigation/drawer';

import CustomDrawer from '../components/CustomDrawer';

import TabNavigator from './TabNavigator';

const Drawer = createDrawerNavigator();

const AppStack = () => {
  return (
    <Drawer.Navigator
      drawerContent={props => <CustomDrawer {...props} />}
      screenOptions={{
        headerShown: false,
        drawerActiveBackgroundColor: '#aa18ea',
        drawerActiveTintColor: '#fff',
        drawerInactiveTintColor: '#333',
        drawerLabelStyle: {
          marginLeft: 10,
          fontFamily: 'Roboto-Medium',
          fontSize: 18,
        },
      }}>
      <Drawer.Screen name="Thủ Đức" component={TabNavigator} />
      <Drawer.Screen name="Hàng Xanh" component={TabNavigator} />
      <Drawer.Screen name="Bảy Hiền" component={TabNavigator} />
    </Drawer.Navigator>
  );
};

export default AppStack;

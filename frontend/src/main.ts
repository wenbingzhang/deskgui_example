import { createApp } from 'vue';
import Antd from 'ant-design-vue';
import App from './App.vue';
import 'ant-design-vue/dist/reset.css';
import Router from './router';

createApp(App).use(Antd).use(Router).mount('#app');

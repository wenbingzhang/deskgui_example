<template>
  <a-config-provider :theme="themeConfig">
    <div class="content">
      <div class="left-background">
        <a-input-search v-model:value="searchValue" style="padding: 8px 5px 8px 5px" placeholder="Search" />
        <a-tree
          :expanded-keys="expandedKeys"
          :auto-expand-parent="autoExpandParent"
          :tree-data="treeData"
          :style="{ padding: '24px 16px 0', overflow: 'initial' }"
          @expand="onExpand"
          @select="onSelect"
        >
          <template #title="{ title }">
            <span v-if="title?.indexOf(searchValue) > -1">
              {{ title.substring(0, title.indexOf(searchValue)) }}
              <span style="color: #f50">{{ searchValue }}</span>
              {{ title.substring(title.indexOf(searchValue) + searchValue.length) }}
            </span>
            <span v-else>{{ title }}</span>
          </template>
        </a-tree>
      </div>
      <div class="right-background">
        <RouterView />
      </div>
    </div>
  </a-config-provider>
</template>
<script lang="ts" setup>
import { ref, watch, computed, nextTick } from 'vue';
import type { TreeProps } from 'ant-design-vue';
import { theme } from 'ant-design-vue';
import router from './router';

const themeConfig = computed(() => ({
  algorithm: theme.darkAlgorithm,
  token: {
    colorPrimary: '#1677ff', // 自定义主题色
  },
}));
document.body.setAttribute('data-theme', 'dark');

const expandedKeys = ref<(string | number | undefined)[]>([]);
const searchValue = ref<string>('');
const autoExpandParent = ref<boolean>(true);
const treeData = ref<TreeProps['treeData']>([
  {
    title: '关于',
    key: 'about',
    // children
  },
]);

const getExpandedKey = (title: string | number, tree: TreeProps['treeData'] = []): (string | number)[] => {
  let expandedKey: (string | number)[] = [];
  for (let i = 0; i < tree.length; i++) {
    const node = tree[i];
    if (node.children) {
      if (node.children.some((item) => item.title.indexOf(title) > -1)) {
        expandedKey.push(node.key);
      } else if (getExpandedKey(title, node.children)) {
        expandedKey.push(...getExpandedKey(title, node.children));
      }
    }
  }
  return expandedKey;
};

const onExpand = (keys: string[]) => {
  expandedKeys.value = keys;
  autoExpandParent.value = false;
};

const onSelect = (keys: string[]) => {
  const addr = keys.length === 1 ? keys[0] : '/';
  router.push(addr);
};

watch(searchValue, (value) => {
  expandedKeys.value = getExpandedKey(value, treeData.value);
  searchValue.value = value;
  autoExpandParent.value = true;
});

nextTick(() => {
  const nv = window as any;
  nv.sys_window('show');
});
</script>

<style>
.content {
  display: flex;
  height: 100vh; /* 不超过视口高度 */
  /* background: #141414; */
}

[data-theme='dark'] .left-background {
  background: #141414;
  color: rgba(255, 255, 255, 0.85);
}

.right-background {
  margin-left: 5px;
  width: 100%;
  padding: 8px;
}

[data-theme='dark'] .right-background {
  background: #141414;
  color: rgba(255, 255, 255, 0.85);
}

[data-theme='dark'] .ant-tree .ant-tree-node-content-wrapper.ant-tree-node-selected {
  color: #1677ff;
  background-color: transparent;
}
</style>

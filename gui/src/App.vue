<script setup lang="ts">
  import { ref, nextTick, useTemplateRef } from 'vue'
  const expressions = ref([]);
  const input = ref();

  async function add_expression() {
    expressions.value.push(input.value);
    input.value = undefined;
  }
</script>

<template>
  <div v-for="(_, index) in expressions" :key="index" class="bg-gray-300 h-16 flex items-center">
    <span class="h-12 text-2xl leading-[3rem] text-center w-10 text-green-600"> ${{index+1}}: </span>
    <div type="text" class="h-full w-full text-2xl leading-[3.75rem] pl-2 border-2 border-gray-400"> {{ expressions[index] }} </div>
  </div>

  <div class="bg-gray-300 h-16 flex items-center">
    <span class="h-12 text-2xl leading-[3rem] text-center w-10 text-green-600"> ${{expressions.length+1}}: </span>
    <input type="text" class="h-full w-full bg-transparent outline-none text-2xl pl-2 focus:border-2 border-gray-400 focus-within:border-blue-400" placeholder="Enter expression" v-model="input" @keyup.enter="add_expression" autofocus>
  </div>
</template>



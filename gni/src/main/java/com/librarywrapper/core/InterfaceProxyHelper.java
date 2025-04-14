package com.librarywrapper.core;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public final class InterfaceProxyHelper implements InvocationHandler {
   private final long generatedObjectPointer;
   private final long generatedCallbackPointer;

   public static Object createInterfaceProxy(long generatedObjectPointer, long generatedCallbackPointer, Class<?> interfaceToImplement) {
      InterfaceProxyHelper proxy = new InterfaceProxyHelper(generatedObjectPointer, generatedCallbackPointer);
      return Proxy.newProxyInstance(proxy.getClass().getClassLoader(), new Class[]{interfaceToImplement}, proxy);
   }

   private InterfaceProxyHelper(long generatedObjectPointer, long generatedCallbackPointer) {
      this.generatedObjectPointer = generatedObjectPointer;
      this.generatedCallbackPointer = generatedCallbackPointer;
   }

   public Object invoke(Object proxy, Method method, Object[] arguments) {
      return nativeInvoke(this.generatedObjectPointer, this.generatedCallbackPointer, method.getName(), arguments);
   }

   private static native Object nativeInvoke(long generatedObjectPointer, long generatedCallbackPointer, String methodName, Object[] args);
}

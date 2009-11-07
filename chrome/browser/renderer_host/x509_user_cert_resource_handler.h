// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_RENDERER_HOST_X509_USER_CERT_RESOURCE_HANDLER_H_
#define CHROME_BROWSER_RENDERER_HOST_X509_USER_CERT_RESOURCE_HANDLER_H_

#include <string>

#include "chrome/browser/renderer_host/resource_dispatcher_host.h"
#include "chrome/browser/renderer_host/resource_handler.h"
#include "chrome/browser/download/download_file.h"

// This class handles the "application/x-x509-user-cert" mime-type
// which is a certificate generated by a CA after a previous
// <keygen> form post.

class X509UserCertResourceHandler : public ResourceHandler {
 public:
  X509UserCertResourceHandler(ResourceDispatcherHost* host,
                              URLRequest* request);

  // Not needed, as this event handler ought to be the final resource.
  bool OnRequestRedirected(int request_id, const GURL& url,
                           ResourceResponse* resp, bool* defer);

  // Check if this indeed an X509 cert.
  bool OnResponseStarted(int request_id, ResourceResponse* resp);

  // Create a new buffer to store received data.
  bool OnWillRead(int request_id, net::IOBuffer** buf, int* buf_size,
                  int min_size);

  // A read was completed, maybe allocate a new buffer for further data.
  bool OnReadCompleted(int request_id, int* bytes_read);

  // Done downloading the certificate.
  bool OnResponseCompleted(int request_id,
                           const URLRequestStatus& urs,
                           const std::string& sec_info);


 private:
  ~X509UserCertResourceHandler() {}

  void AssembleResource();

  GURL url_;
  ResourceDispatcherHost* host_;
  URLRequest* request_;
  size_t content_length_;
  scoped_ptr<DownloadBuffer> buffer_;
  scoped_refptr<net::IOBuffer> read_buffer_;
  scoped_refptr<net::IOBuffer> resource_buffer_;  // Downloaded certificate.
  static const int kReadBufSize = 32768;

  DISALLOW_COPY_AND_ASSIGN(X509UserCertResourceHandler);
};

#endif  // CHROME_BROWSER_RENDERER_HOST_X509_USER_CERT__RESOURCE_HANDLER_H_
